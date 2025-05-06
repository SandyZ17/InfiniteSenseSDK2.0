#pragma once
#include <cstdint>
#include <string>
#include "image.h"
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
  IMU_1 = 0,  // internal imu
  IMU_2 = 1,  // external imu
  CAM_1 = 2,  // camera 1
  CAM_2 = 3,  // camera 2
  CAM_3 = 4,  // camera 3
  CAM_4 = 5,  // camera 4
  LASER = 6,  // laser pps
  GPS = 7,    // gps pps
};
}  // namespace infinite_sense
