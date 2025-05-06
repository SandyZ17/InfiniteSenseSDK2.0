#include "infinite_sense.h"
using namespace infinite_sense;
int main() {
  Synchronizer synchronizer;
  /*
   使用网口连接
   synchronizer.SetNetLink("192.168.1.188", 8888);
  */
  // 使用USB连接
  synchronizer.SetSerialLink("/dev/ttyACM0", 460800);
  /*
    如使用工业相机, 需要指定 相机名称 和 同步板的触发端口
    std::map<std::string, TriggerDevice> params;
    params["camera_1"] = TriggerDevice::CAM_1; //camera_1:表示设备的名称，TriggerDevice::CAM_1:使用同步板CAM_1端口触发
    synchronizer.UseMvCam(params);
  */

  // 开启同步
  synchronizer.Start();
  Synchronizer::PrintSummary();
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds{1000});
  }
  // 停止同步
  synchronizer.Stop();
  return 0;
}