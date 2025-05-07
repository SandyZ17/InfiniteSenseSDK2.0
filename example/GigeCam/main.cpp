#include "infinite_sense.h"
#include "mv_cam.h"
using namespace infinite_sense;
void ImuCallback(const void *msg,size_t) {

}
void ImageCallback(const void *msg,size_t) {

}
int main() {
  // 1.创建同步器
  Synchronizer synchronizer;
  synchronizer.SetUsbLink("/dev/ttyACM0", 460800);

  // 2.配置同步接口
  auto mv_cam = std::make_shared<MvCam>();
  mv_cam->SetParams({{"camera_1", CAM_1}});
  synchronizer.UseCam(mv_cam);

  // 3.开启同步
  synchronizer.Start();

  // 4.接收数据
  Messenger::GetInstance().SubStruct("imu_1", ImuCallback);
  Messenger::GetInstance().SubStruct("camera_1", ImageCallback);
  // 5.停止同步
  synchronizer.Stop();
  return 0;
}