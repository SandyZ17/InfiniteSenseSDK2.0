#include "infinite_sense.h"
#include <thread>
using namespace infinite_sense;
int main(int argc, char **argv)
{
  if (argc < 2){
    std::cerr << "Usage: ./infinite_sense_node <config_file>" << std::endl;
  }
  std::string config_path = argv[1];
  std::cout<<"Read config from "<<config_path<<std::endl;
  Synchronizer synchronizer(config_path);
  synchronizer.PrintSummary();
  synchronizer.Start();
  while (true) {
    if (synchronizer.ImuUpdate()) {
        ImuData imu = synchronizer.GetImuData();
        LOG(INFO) << "ImuData: {" 
        << " time_stamp_us: " << imu.time_stamp_us
        << ", temperature: " << imu.temperature
        << ", name: " << imu.name
        << ", a: [" << imu.a[0] << ", " << imu.a[1] << ", " << imu.a[2] << "]"
        << ", g: [" << imu.g[0] << ", " << imu.g[1] << ", " << imu.g[2] << "]"
        << ", q: [" << imu.q[0] << ", " << imu.q[1] << ", " << imu.q[2] << ", " << imu.q[3] << "]"
        << " }";
    }
    
    GPSData gps = synchronizer.GetGPSData();
    LOG(INFO) << "GPSData: {"
    << " time_stamp_us: " << gps.time_stamp_us
    << ", gps_stamp_us: " << gps.gps_stamp_us
    << ", gps_stamp_us_trigger: " << gps.gps_stamp_us_trigger
    << ", name: \"" << gps.name << "\""
    << ", latitude: " << gps.latitude
    << ", longitude: " << gps.longitude
    << " }";

    TriggerData trigger = synchronizer.GetTriggerData();
    LOG(INFO) << "TriggerData: {"
    << " cam_time_stamp_us: " << trigger.cam_time_stamp_us
    << ", dev_1_time_stamp_us: " << trigger.dev_1_time_stamp_us
    << ", dev_2_time_stamp_us: " << trigger.dev_2_time_stamp_us
    << ", dev_3_time_stamp_us: " << trigger.dev_3_time_stamp_us
    << " }";
    
    std::this_thread::sleep_for(std::chrono::milliseconds{250});
  }
  synchronizer.Stop();
  return 0;
}