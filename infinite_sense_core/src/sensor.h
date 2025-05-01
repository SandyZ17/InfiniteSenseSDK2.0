#pragma once
#include "json.hpp"
#include "infinite_sense.h"
#include "trigger.h"

namespace infinite_sense {

inline void ProcessTriggerData(const nlohmann::json &data) {
  if (data["f"] != "t") {
    return;
  }
  const uint64_t time_stamp = data["t"];
  const uint16_t status = data["s"];
  const uint64_t count  =data["c"];
  SET_LAST_TRIGGER_STATUS(time_stamp, status);
};

inline void ProcessIMUData(const nlohmann::json &data) {
  if (data["f"] != "imu") {
    return;
  }
  ImuData imu{};
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
  Messenger::GetInstance().PubStruct("imu1",&imu,sizeof(imu));
};

inline void ProcessGPSData(const nlohmann::json &data) {
  if (data["f"] != "GNGGA") {
    return;
  }
  GPSData gps{};
  gps.latitude = data["d"][0];
  gps.longitude = data["d"][1];
  gps.gps_stamp_us = data["d"][2];
  gps.gps_stamp_us_trigger = data["d"][3];
  gps.time_stamp_us = data["t"];
  Messenger::GetInstance().PubStruct("gps",&gps,sizeof(gps));
};


inline void ProcessLOGData(const nlohmann::json &data) {
  if (data["f"] != "log") {
    return;
  }
  LOG(data["l"]) << data["msg"];
};

}  // namespace infinite_sense