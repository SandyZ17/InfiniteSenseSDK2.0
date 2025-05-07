#include "infinite_sense.h"
#include "mv_cam.h"
using namespace infinite_sense;
int main() {

  // 1.创建同步器
  Synchronizer synchronizer;
  /*
   使用网口连接
   synchronizer.SetNetLink("192.168.1.188", 8888);
  */
  synchronizer.SetUsbLink("/dev/ttyACM0", 460800);
  // 如使用工业相机, 需要指定 相机名称 和 同步板的触发端口
  std::map<std::string, TriggerDevice> params;
  params["camera_1"] = TriggerDevice::CAM_1; // camera_1:表示设备的名称，TriggerDevice::CAM_1:使用同步板CAM_1端口触发
  const auto mv_cam = std::make_shared<MvCam>(params);
  synchronizer.UseCam(mv_cam);

  // 2.开启同步
  synchronizer.Start();

  // 3.订阅数据
  Synchronizer::PrintSummary();
  zmq::context_t context(1);
  zmq::socket_t subscriber(context, zmq::socket_type::sub);
  subscriber.connect("tcp://localhost:5555");

  const std::string topic = "imu_1";   // 订阅特定主题（如,板载IMU数据： "imu_1"）
  subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
  zmq::message_t msg;
  while (true) {
    if (subscriber.recv(msg, zmq::recv_flags::dontwait)) {
      if (subscriber.get(zmq::sockopt::rcvmore)) {
        zmq::message_t dummy;
        subscriber.recv(dummy);
      }
    }
  }
  // 4.停止同步
  synchronizer.Stop();
  return 0;
}