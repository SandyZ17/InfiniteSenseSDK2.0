#pragma once
#include "infinite_sense.h"
namespace infinite_sense {
class CustomCam final : public Sensor {
 public:
  explicit CustomCam(const std::map<std::string, TriggerDevice>& params) : params_(params) {}
  ~CustomCam() override;

  CustomCam(const CustomCam&) = delete;
  CustomCam& operator=(const CustomCam&) = delete;

  bool Initialization() override;
  void Stop() override;
  void Start() override;

 private:
  void Receive(void* handle, const std::string&) override;
};
}  // namespace infinite_sense
