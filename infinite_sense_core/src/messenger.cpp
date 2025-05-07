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

}  // namespace infinite_sense