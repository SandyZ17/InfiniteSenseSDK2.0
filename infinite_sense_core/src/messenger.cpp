#include "messenger.h"
#include "log.h"
namespace infinite_sense {
Messenger::Messenger() {
  endpoint_ = "tcp://127.0.0.1:4565";
  publisher_.close();
  subscriber_.close();
  context_.close();
  context_ = zmq::context_t(10);
  publisher_ = zmq::socket_t(context_, ZMQ_PUB);
  subscriber_ = zmq::socket_t(context_, ZMQ_SUB);
  try {
    publisher_.connect(endpoint_);
    subscriber_.connect(endpoint_);
    LOG(INFO) << "New ZMQ endpoint: " << endpoint_;
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Failed to bind ZMQ endpoint: " << e.what();
    context_.close();
    publisher_.close();
    subscriber_.close();
  }
};
Messenger::~Messenger() {
  publisher_.close();
  subscriber_.close();
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
void Messenger::Sub(const std::string &topic, const std::function<void(const std::string &)>& callback) {
  try {
    subscriber_.set(zmq::sockopt::subscribe, topic);
    while (true) {
      zmq::message_t topic_msg;
      zmq::message_t data_msg;
      subscriber_.recv(topic_msg, zmq::recv_flags::none);
      std::string received_topic(static_cast<char*>(topic_msg.data()), topic_msg.size());
      if (received_topic != topic) {
        continue;
      }
      subscriber_.recv(data_msg, zmq::recv_flags::none);
      std::string data(static_cast<char*>(data_msg.data()), data_msg.size());
      callback(data);
    }
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Exception in Sub: " << e.what();
  }
}
void Messenger::SubStruct(const std::string &topic, const std::function<void(const void *, size_t)>& callback) {
  try {
    subscriber_.set(zmq::sockopt::subscribe, topic);
    while (true) {
      zmq::message_t topic_msg;
      zmq::message_t data_msg;
      subscriber_.recv(topic_msg, zmq::recv_flags::none);
      std::string received_topic(static_cast<char*>(topic_msg.data()), topic_msg.size());
      if (received_topic != topic) {
        continue;
      }
      subscriber_.recv(data_msg, zmq::recv_flags::none);
      callback(data_msg.data(), data_msg.size());
    }
  } catch (const zmq::error_t &e) {
    LOG(ERROR) << "Exception in SubStruct: " << e.what();
  }
}

}  // namespace infinite_sense