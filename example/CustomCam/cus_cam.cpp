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
  for (auto &cam_thread : cam_threads) {
    while (cam_thread.joinable()) {
      cam_thread.join();
    }
  }
  cam_threads.clear();
  cam_threads.shrink_to_fit();
}
void CustomCam::Receive(void *handle, const std::string &name)  {
  Messenger &messenger = Messenger::GetInstance();
  while (is_running) {
      CamData cam_data;
      if (params.find(name) != params.end()) {
        if (uint64_t time; GET_LAST_TRIGGER_STATUS(params[name], time)) {
          cam_data.time_stamp_us = time;
        }
      }
      cam_data.image = GMat();
      messenger.PubStruct(name,&cam_data,sizeof(cam_data));
      std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
}
void CustomCam::Start() {
  std::string name = "cus_camera";
  cam_threads.emplace_back(&CustomCam::Receive, this, nullptr, name);
}
}  // namespace infinite_sense