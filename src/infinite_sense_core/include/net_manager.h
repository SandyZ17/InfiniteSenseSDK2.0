#pragma once
#include "practical_socket.h"
#include "sensor.h"
#include <thread>
#include <memory>

namespace infinite_sense {
class Ptp;
class SynchronizerData;
class NetManager {
 public:
  explicit NetManager(std::string target_ip, unsigned short port, std::shared_ptr<SynchronizerData> data);

  ~NetManager();

  void Start();

  void Stop();

 private:

  void Receive();

  void TimeStampSynchronization() const;
  std::shared_ptr<UDPSocket> net_ptr_;
  std::shared_ptr<Ptp> ptp_;

  unsigned short port_{};
  std::string target_ip_;

  std::thread rx_thread_, tx_thread_;
  bool started_{false};
  size_t frame_count_{0};
  bool config_{false};

  std::shared_ptr<SynchronizerData> data_;
  ImuData imu_data_;
  GPSData gps_data_;
  TriggerData trigger_data_{};
};
}  // namespace infinite_sense
