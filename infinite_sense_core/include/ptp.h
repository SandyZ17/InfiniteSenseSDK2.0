#pragma once
#include <json.hpp>
#include "ser.h"
#include <practical_socket.h>
namespace infinite_sense {
class Ptp {
 public:
  Ptp() = default;
  void ReceivePtpData(const nlohmann::json &data);
  void SendPtpData() const;
  void SetSerialPtr(const std::shared_ptr<serial::Serial> &serial_ptr);
  void SetNetPtr(const std::shared_ptr<UDPSocket> &net_ptr, const std::string &target_ip, unsigned short port);

 private:
  std::shared_ptr<serial::Serial> serial_ptr_{nullptr};
  std::shared_ptr<UDPSocket> net_ptr_{nullptr};
  unsigned short port_{};
  std::string target_ip_{};
  uint64_t time_t1_{0};
  uint64_t time_t2_{0};
  bool updated_t1_t2_{false};
};

}  // namespace infinite_sense