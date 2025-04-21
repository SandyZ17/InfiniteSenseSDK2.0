#pragma once
#include "serial.h"

#include <thread>
#include <memory>
#include "sensor.h"

namespace infinite_sense {
class Ptp;
class SerialManager {
 public:
  explicit SerialManager(const std::string &port, int baud_rate, std::shared_ptr<SynchronizerData> data);

  ~SerialManager();

  [[nodiscard]] bool IsAvailable() const { return serial_ptr_->isOpen(); }

  void Start();

  void Stop();

 private:

  void Receive();

  void TimeStampSynchronization() const;
  std::string port_;
  std::shared_ptr<serial::Serial> serial_ptr_;
  std::shared_ptr<Ptp> ptp_;
  std::thread rx_thread_, tx_thread_;
  bool started_{false};
  size_t frame_count_{0};

  std::shared_ptr<SynchronizerData> data_;
  ImuData imu_data_;
  GPSData gps_data_;
  TriggerData trigger_data_{};
};
}  // namespace infinite_sense
