#pragma once
#include "log.h"
#include <mutex>
#include "sensor.h"

namespace infinite_sense {

class NetManager;
class SerialManager;
class TriggerManger;
class Messenger;

class Parameters {
 public:
  std::string log_dir;
  std::string time_base; 
  std::string communication_type;
  
  int udp_port = 8888;
  std::string udp_ip  = "[192,168,1,188]";
  std::string udp_subnet = "[255,255,255,0]";
  
  std::string device = "/dev/ttyACM0";
  int usb_baud_rate = 921600;

  int io_camera_hz = 30;
  bool io_camera_reverse = false;
  int io_camera_duration = 5;

  std::string io_device_mode_1 = "output";
  int io_device_hz_1 = 30;
  bool io_device_reverse_1 = false;
  int io_device_duration_1 = 5;
  int io_device_max_frequency_1 = 50;

  std::string io_device_mode_2 = "output";
  int io_device_hz_2 = 40;
  bool io_device_reverse_2 = true;
  int io_device_duration_2 = 5;
  int io_device_max_frequency_2 = 50;

  
  std::string io_device_mode_3 = "input";
  int io_device_hz_3 = 50;
  bool io_device_reverse_3 = false;
  int io_device_duration_3 = 5;
  int io_device_max_frequency_3 = 50;

  int pps_baud_rate = 9600;
  bool pps_flow_control = false;
  int pps_data_bits = 8;
  int pps_stop_bits = 1;
  int pps_parity = 0;

  int gps_baud_rate = 115200;
  bool gps_flow_control = false;
  int gps_data_bits = 8;
  int gps_stop_bits = 1;
  int gps_parity = 0;
};

class Synchronizer {
 public:
  Synchronizer(std::string config);
  ~Synchronizer() = default;
  void Start() const;
  void Stop() const;
  void SetLogPath(const std::string &path);
  void PrintSummary();
  
  // 获取的数据
  bool ImuUpdate() {return data_ptr_->ImuUpdate();}
  ImuData GetImuData() {return data_ptr_->GetImuData();}
  GPSData GetGPSData() {return data_ptr_->GetGPSData();}
  TriggerData GetTriggerData() {return data_ptr_->GetTriggerData();}
 private:
  std::shared_ptr<NetManager> net_manager_{nullptr};
  std::shared_ptr<SerialManager> serial_manager_{nullptr};

  void readConfigFile(const std::string path);
  std::shared_ptr<Parameters> config_ptr_{};

  std::shared_ptr<SynchronizerData> data_ptr_;
};

}  // namespace infinite_sense 
