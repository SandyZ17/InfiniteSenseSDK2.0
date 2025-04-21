#pragma once
#include <../include/json.hpp>
#include <queue>
#include <mutex>
#include "log.h"

namespace infinite_sense {

typedef struct ImuData {
  uint64_t time_stamp_us;
  float temperature;
  std::string name;
  float a[3];
  float g[3];
  float q[4];
}ImuData;

typedef struct GPSData {
  uint64_t time_stamp_us;
  uint64_t gps_stamp_us;
  uint64_t gps_stamp_us_trigger;
  std::string name;
  float latitude;
  float longitude;
}GPSData;


typedef struct TriggerData {
  uint64_t cam_time_stamp_us;
  uint64_t dev_1_time_stamp_us;
  uint64_t dev_2_time_stamp_us;
  uint64_t dev_3_time_stamp_us;
}TriggerData;

struct SynchronizerData{
public:
  std::queue<ImuData> imu_queue_;
  GPSData gps_;
  TriggerData trigger_;
  std::mutex imu_mutex_;
  std::mutex gps_mutex_;
  std::mutex trigger_mutex_;

  ImuData GetImuData() {
    imu_mutex_.lock(); 
    ImuData tmp = imu_queue_.front();
    imu_queue_.pop();
    imu_mutex_.unlock();
    return tmp;
  }

  GPSData GetGPSData() {
    gps_mutex_.lock();
    GPSData tmp = gps_;
    gps_mutex_.unlock();
    return tmp;
  }

  TriggerData GetTriggerData() {
    trigger_mutex_.lock();
    TriggerData tmp = trigger_;
    gps_mutex_.unlock();
    return tmp;
  }

  void AddImuData(ImuData tmp){
    imu_mutex_.lock(); 
    imu_queue_.push(tmp);
    if (imu_queue_.size() >= 50) {
      imu_queue_.pop();
    }
    imu_mutex_.unlock();
  }

  void SetGPSData(GPSData tmp) {
    gps_mutex_.lock();
    gps_ = tmp;
    gps_mutex_.unlock();
  }

  void SetTriggerData(TriggerData tmp) {
    trigger_mutex_.lock();
    trigger_ = tmp;
    gps_mutex_.unlock();
  }
  
  bool ImuUpdate() {return !imu_queue_.empty();}
};

inline void ProcessTriggerData(const nlohmann::json &data, TriggerData &trigger) {
  if (data["f"] != "t") {
    return;
  }
  const uint64_t time_stamp = data["t"];
  const uint16_t status = data["s"];
  const uint64_t count  =data["c"];
//   SET_LAST_TRIGGER_STATUS(time_stamp, status);
  trigger.cam_time_stamp_us = time_stamp;
  trigger.dev_1_time_stamp_us = time_stamp;
  trigger.dev_2_time_stamp_us = time_stamp;
  trigger.dev_3_time_stamp_us = time_stamp;
};

inline bool ProcessIMUData(const nlohmann::json &data, ImuData &imu) {
  if (data["f"] != "imu") {
    return false;
  }
  const uint64_t time_stamp = data["t"];
  const uint64_t count = data["c"];
  imu.time_stamp_us = time_stamp;
  // std::cout << imu.time_stamp_us << std::endl;
  imu.a[0] = data["d"][0];
  imu.a[1] = data["d"][1];
  imu.a[2] = data["d"][2];
  imu.g[0] = data["d"][3];
  imu.g[1] = data["d"][4];
  imu.g[2] = data["d"][5];
  imu.temperature = data["d"][6];
  imu.q[0] = data["q"][0];
  imu.q[1] = data["q"][1];
  imu.q[2] = data["q"][2];
  imu.q[3] = data["q"][3];
  return true;
};

inline void ProcessGPSData(const nlohmann::json &data, GPSData &gps) {
  if (data["f"] != "GNGGA") {
    return;
  }
  gps.latitude = data["d"][0];
  gps.longitude = data["d"][1];
  gps.gps_stamp_us = data["d"][2];
  gps.gps_stamp_us_trigger = data["d"][3];
  gps.time_stamp_us = data["t"];
};


inline void ProcessLOGData(const nlohmann::json &data) {
  if (data["f"] != "log") {
    return;
  }
  LOG(data["l"]) << data["msg"];
};

}  // namespace infinite_sense