#pragma once
#include <thread>
#include <vector>
#include "messenger.h"
namespace infinite_sense {
class Sensor{
public:
  Sensor();
  explicit Sensor(const std::map<std::string, TriggerDevice>& params) : params_(params) {}
  virtual ~Sensor();

  Sensor(const Sensor&) = delete;
  Sensor& operator=(const Sensor&) = delete;

  virtual bool Initialization();
  virtual void Stop();
  virtual void Start();
  virtual void Restart() {
      Stop();
      std::this_thread::sleep_for(std::chrono::milliseconds{500});
      if (!Initialization()) {
        LOG(INFO) << "Camera initialization failed after restart!";
      } else {
        Start();
        LOG(INFO) << "Cameras successfully restarted!";
      }
  }
  void Enable() { is_running_ = true; }
  void Disable() { is_running_ = false; }
private:
  virtual void Receive(void* handle, const std::string&);
  bool is_running_{false};
  std::vector<std::thread> cam_threads_{};
  std::map<std::string, TriggerDevice> params_{};
};

}  // namespace infinite_sense
