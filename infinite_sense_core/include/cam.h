#pragma once
#include <thread>
#include <vector>
#include "messenger.h"
#include "infinite_sense.h"
namespace infinite_sense {
class CamManger {
 public:
  explicit CamManger(const std::map<std::string, TriggerDevice>& params) : params_(params) {}
  ~CamManger();

  CamManger(const CamManger&) = delete;
  CamManger& operator=(const CamManger&) = delete;

  bool Initialization();
  void Stop();
  void Start();
  void Enable() { is_running_ = true; }
  void Disable() { is_running_ = false; }
  void Restart();

 private:
  void Receive(void* handle, const std::string&) const;
  bool is_running_{false};
  std::vector<int> rets_;
  std::vector<void*> handles_;
  std::vector<std::thread> cam_threads_;
  std::map<std::string, TriggerDevice> params_;
};
}  // namespace infinite_sense
