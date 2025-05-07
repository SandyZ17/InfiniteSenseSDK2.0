#pragma once
#include <atomic>
#include <log.h>
#include <mutex>
#include <thread>
#include <zmq.hpp>
#include <unordered_set>
#include <unordered_map>
namespace infinite_sense {
class Messenger {
 public:
  static Messenger& GetInstance() {
    static Messenger instance;
    return instance;
  }
  Messenger(const Messenger&) = delete;
  Messenger(const Messenger&&) = delete;
  Messenger& operator=(const Messenger&) = delete;
  std::string GetPubEndpoint() const;
  void Pub(const std::string& topic, const std::string& metadata);
  void PubStruct(const std::string& topic, const void* data, size_t size);
  [[noreturn]] void WaitAsk();
 private:
  Messenger();
  ~Messenger();
  zmq::context_t context_{};
  zmq::socket_t publisher_{};
  std::string endpoint_{};
 private:
  zmq::socket_t asker_{};
  std::thread ask_thread_;
};
}  // namespace infinite_sense
