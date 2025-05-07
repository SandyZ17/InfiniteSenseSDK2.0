#include "messenger.h"
#include "log.h"
namespace infinite_sense {
Messenger::Messenger() {
  endpoint_ = "tcp://*:4565";
  publisher_.close();
  context_.close();
  context_ = zmq::context_t(1);
  publisher_ = zmq::socket_t(context_, ZMQ_PUB);
  publisher_.set(zmq::sockopt::linger, 0);
  publisher_.set(zmq::sockopt::rcvtimeo, 1000);
  try {
    publisher_.bind(endpoint_);
    // publisher_.bind("tcp://*:0");
    // endpoint_ = publisher_.get(zmq::sockopt::last_endpoint);
    LOG(INFO) << "Publisher: " << endpoint_;
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Failed to bind ZMQ publisher: " << e.what();
    context_.close();
    publisher_.close();
  }
  asker_ = zmq::socket_t(context_, zmq::socket_type::rep);
  asker_.bind("tcp://*:4564");
  ask_thread_ = std::thread([this] { WaitAsk(); });
};
Messenger::~Messenger() {
  publisher_.close();
  while (ask_thread_.joinable()) {
    ask_thread_.join();
  }
}
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

[[noreturn]] void Messenger::WaitAsk() {
  while (true) {
    zmq::message_t request;
    asker_.recv(request, zmq::recv_flags::none);
    std::string received_msg(static_cast<char *>(request.data()), request.size());
    if (received_msg == "ask_endpoint") {
      std::string reply_msg = GetPubEndpoint();
      zmq::message_t reply(reply_msg.size());
      memcpy(reply.data(), reply_msg.data(), reply_msg.size());
      asker_.send(reply, zmq::send_flags::none);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}
}  // namespace infinite_sense