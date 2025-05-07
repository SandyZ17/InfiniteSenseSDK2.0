#pragma once
#include "usb.h"
#include "serial.h"
#include <thread>
#include <memory>

namespace infinite_sense {
class Ptp;
class UsbManager {
 public:
  explicit UsbManager(const std::string &port, int baud_rate);

  ~UsbManager();

  [[nodiscard]] bool IsAvailable() const { return serial_ptr_->isOpen(); }

  void Start();

  void Stop();

 private:

  void Receive() const;

  void TimeStampSynchronization() const;
  std::string port_;
  std::shared_ptr<serial::Serial> serial_ptr_;
  std::shared_ptr<Ptp> ptp_;
  std::thread rx_thread_, tx_thread_;
  bool started_{false};
  size_t frame_count_{0};
};
}  // namespace infinite_sense
