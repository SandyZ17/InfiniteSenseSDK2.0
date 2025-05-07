#pragma once
#include "infinite_sense.h"
namespace infinite_sense {
class CamManger final : public Cam {
 public:
  explicit CamManger(const std::map<std::string, TriggerDevice>& params) : params_(params) {}
  ~CamManger() override;

  bool Initialization() override;
  void Stop() override;
  void Start() override;

 private:
  void Receive(void* handle, const std::string&) override;
  bool is_running_{false};
  std::vector<int> rets_;
  std::vector<void*> handles_;
  std::vector<std::thread> cam_threads_;
  std::map<std::string, TriggerDevice> params_;
};
}  // namespace infinite_sense
