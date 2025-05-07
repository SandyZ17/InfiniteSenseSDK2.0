#include "infinite_sense.h"
#include "mv_cam.h"
using namespace infinite_sense;
int main() {
  // 1.创建同步器
  Synchronizer synchronizer;
  synchronizer.SetUsbLink("/dev/ttyACM0", 460800);
  // 2.配置同步接口
  std::map<std::string, TriggerDevice> params = {{"camera_1", CAM_1}};
  auto mv_cam = std::make_shared<MvCam>();
  mv_cam->SetParams(params);
  synchronizer.UseCam(mv_cam);

  // 3.开启同步
  synchronizer.Start();

  // 4.接受数据
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
  // 5.停止同步
  synchronizer.Stop();
  return 0;
}