#pragma once
#include <cstdint>
#include <string>
#include "image.h"
namespace infinite_sense {
struct ImuData {
  uint64_t time_stamp_us; // microseconds since start of recording
  float temperature;      // temperature in Celsius
  std::string name;       // imu name
  float a[3];             // accelerometer
  float g[3];             // gyroscope
  float q[4];             // quaternion
};

struct CamData {
  uint64_t time_stamp_us;   // microseconds since start of recording
  std::string name;         // camera name
  GMat image;               // image data
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
//
std::map<TriggerDevice, std::string> device_topics{
    {IMU_1, "imu_1_trigger"},
    {IMU_2, "imu_2_trigger"},
    {CAM_1, "cam_1_trigger"},
    {CAM_2, "cam_2_trigger"},
    {CAM_3, "cam_3_trigger"},
    {CAM_4, "cam_4_trigger"},
    {LASER, "laser_trigger"},
    {GPS, "gps_trigger"}};
}  // namespace infinite_sense
