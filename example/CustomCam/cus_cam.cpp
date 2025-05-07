#include "cus_cam.h"
#include "infinite_sense.h"
namespace infinite_sense {

 CustomCam::~CustomCam() {
  Stop();
 }

bool CustomCam::Initialization() {
  return true;
}

void CustomCam::Stop() {
  Disable();
  std::this_thread::sleep_for(std::chrono::milliseconds{500});
  for (auto &cam_thread : cam_threads_) {
    while (cam_thread.joinable()) {
      cam_thread.join();
    }
  }
  cam_threads_.clear();
  cam_threads_.shrink_to_fit();
}
void CustomCam::Receive(void *handle, const std::string &name)  {
  while (is_running_) {
      // 这里的time_stamp_us是相机触发时间，需要加上曝光时间的一半，以获得相机拍摄的时间
      if (params_.find(name) == params_.end()) {
        LOG(ERROR) << "cam " << name << " not found!";
      }
      else {
        if (uint64_t time; GET_LAST_TRIGGER_STATUS(params_[name], time)) {
          cam_data.time_stamp_us = time;
        }
        else {
          LOG(ERROR) << "cam " << name << " not found!";
        }
      }

      cam_data.image = GMat(st_out_frame.stFrameInfo.nHeight, st_out_frame.stFrameInfo.nWidth,
                                GMatType<uint8_t, 1>::Type, st_out_frame.pBufAddr);

      messenger.PubStruct(name,&cam_data,sizeof(cam_data));
      std::this_thread::sleep_for(std::chrono::milliseconds{2});
    }
}
void CustomCam::Start() {
  std::string name = "cus_camera";
  cam_threads_.emplace_back(&CustomCam::Receive, this, nullptr, name);
}
}  // namespace infinite_sense