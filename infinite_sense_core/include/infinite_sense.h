#pragma once
#include "log.h"
#include "image.h"
#include "messenger.h"
namespace infinite_sense {

struct ImuData {
  uint64_t time_stamp_us;
  float temperature;
  std::string name;
  float a[3];
  float g[3];
  float q[4];
};

struct CamData {
  uint64_t time_stamp_us;
  std::string name;
  GMat image;
};

struct LaserData {
  uint64_t time_stamp_us;
  std::string name;
};

struct GPSData {
  uint64_t time_stamp_us;
  uint64_t gps_stamp_us;
  uint64_t gps_stamp_us_trigger;
  std::string name;
  float latitude;
  float longitude;
};

enum TriggerDevice {
  IMU_1 = 0,    // internal imu
  IMU_2 = 1,    // external imu
  CAM_1 = 2,    // camera 1
  CAM_2 = 3,    // camera 2
  CAM_3 = 4,    // camera 3
  CAM_4 = 5,    // camera 4
  LASER = 6,    // laser pps
  GPS   = 7,    // gps pps
};
class NetManager;
class SerialManager;
class CamManger;
class TriggerManger;
class Messenger;
class Synchronizer {
 public:
  Synchronizer();
  ~Synchronizer() = default;
  void Start() const;
  void Stop() const;
  static void SetLogPath(const std::string &path);
  void SetSerialLink(std::string serial_dev, int serial_baud_rate);
  void SetNetLink(std::string net_dev, unsigned int port);
  void UseMvCam(const std::map<std::string, TriggerDevice>& params = std::map<std::string, TriggerDevice>());
  static bool GetLastTriggerTime(TriggerDevice dev, uint64_t time);
  static void PrintSummary() ;
 private:
  std::string net_ip_;
  unsigned short net_port_{};
  std::string serial_dev_;
  int serial_baud_rate_{};
  std::shared_ptr<NetManager> net_manager_{nullptr};
  std::shared_ptr<SerialManager> serial_manager_{nullptr};
  std::shared_ptr<CamManger> cam_manager_{nullptr};
};

}  // namespace infinite_sense
