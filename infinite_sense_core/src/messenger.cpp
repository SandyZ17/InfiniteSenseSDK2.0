#include "messenger.h"
#include "log.h"
namespace infinite_sense {
Messenger::Messenger() {
  publisher_.close();
  context_.close();
  context_ = zmq::context_t(1);
  publisher_ = zmq::socket_t(context_, ZMQ_PUB);
  publisher_.set(zmq::sockopt::linger, 0);
  publisher_.set(zmq::sockopt::rcvtimeo, 1000);
  try {
    publisher_.bind("tcp://*:0");
    endpoint_ = publisher_.get(zmq::sockopt::last_endpoint);
    LOG(INFO) << "ZMQ PUB: " << endpoint_;
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Failed to bind ZMQ publisher: " << e.what();
    context_.close();
    publisher_.close();
  }
}
Messenger::~Messenger() { publisher_.close(); }
void Messenger::Pub(const std::string &topic, const std::string &metadata) {
  try {
    zmq::message_t topic_msg(topic.size());
    memcpy(topic_msg.data(), topic.c_str(), topic.size());
    publisher_.send(topic_msg, zmq::send_flags::sndmore);
    zmq::message_t query(metadata.length());
    memcpy(query.data(), metadata.c_str(), metadata.size());
    publisher_.send(query, zmq::send_flags::dontwait);
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Exception: " << e.what();
  }
}

void Messenger::PubStruct(const std::string &topic, const void *data, const size_t size) {
  try {
    zmq::message_t topic_msg(topic.size());
    memcpy(topic_msg.data(), topic.c_str(), topic.size());
    publisher_.send(topic_msg, zmq::send_flags::sndmore);

    zmq::message_t data_msg(size);
    memcpy(data_msg.data(), data, size);
    publisher_.send(data_msg, zmq::send_flags::dontwait);
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Exception: " << e.what();
  }
}
std::string Messenger::GetPubEndpoint() const { return endpoint_; }


std::unordered_set<std::string> TopicMonitor::GetTopics() const {
  std::lock_guard lock(topics_mutex_);
  return topics_;
}
void TopicMonitor::Start() {
  if (monitor_thread_.joinable()) {
    return;
  }
  should_run_.store(true);
  monitor_thread_ = std::thread(&TopicMonitor::MonitorLoop, this);
}

void TopicMonitor::Stop() {
  if (!should_run_.load()) {
    return;
  }
  should_run_.store(false);
  if (monitor_thread_.joinable()) {
    monitor_thread_.join();
  }
}
TopicMonitor::TopicMonitor() : context_(1), subscriber_(context_, ZMQ_SUB), should_run_(false) {
  try {
    subscriber_.connect(Messenger::GetInstance().GetPubEndpoint());
    subscriber_.set(zmq::sockopt::subscribe, "");
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "[TopicMonitor] Initialization failed: " << e.what();
    throw;
  }
}
TopicMonitor::~TopicMonitor() {
  Stop();
  try {
    subscriber_.close();
    context_.close();
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "[TopicMonitor] Cleanup error: " << e.what();
  }
}
void TopicMonitor::MonitorLoop() {
  zmq::message_t msg;

  while (should_run_.load()) {
    try {
      if (subscriber_.recv(msg, zmq::recv_flags::dontwait)) {
        {
          std::string topic(static_cast<char *>(msg.data()), msg.size());
          std::lock_guard lock(topics_mutex_);
          topic_frequencies_[topic]++;
        }
        if (subscriber_.get(zmq::sockopt::rcvmore)) {
          zmq::message_t dummy;
          subscriber_.recv(dummy);
        }
      }
    } catch (const zmq::error_t &e) {
      if (e.num() != ETERM) {
      }
    }
  }
}

}  // namespace infinite_sense