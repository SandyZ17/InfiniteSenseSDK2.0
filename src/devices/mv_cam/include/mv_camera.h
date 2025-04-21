#pragma once
#include <thread>
#include <vector>
#include "image.h"
#include "MvCameraControl.h"

namespace infinite_sense {

struct CamData {
    uint64_t time_stamp_us;
    std::string name;
    GMat image;
};

bool IsColor(const MvGvspPixelType type);
bool IsMono(const MvGvspPixelType type);

class MVCamera {
 public:
  explicit MVCamera(){}
  ~MVCamera();

  MVCamera(const MVCamera&) = delete;
  MVCamera& operator=(const MVCamera&) = delete;

  bool Initialization();
  void Stop();
  void Start();
  void Enable() { is_running_ = true; }
  void Disable() { is_running_ = false; }
  void Restart();
  int DeviceNum() {return handles_.size();}
  void* GetHandle(int index) {return handles_[index];}
 private:
  void Receive(void* handle, const std::string&) const;
  std::vector<int> rets_;
  std::vector<void*> handles_;
  std::vector<std::thread> cam_threads_;
  bool is_running_{false};
};
}  // namespace infinite_sense
