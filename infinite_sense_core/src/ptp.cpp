#include "ptp.h"
#include <serial.h>
#include <thread>
namespace infinite_sense {
void Ptp::SetSerialPtr(const std::shared_ptr<serial::Serial> &serial_ptr) { serial_ptr_ = serial_ptr; }
void Ptp::SetNetPtr(const std::shared_ptr<UDPSocket> &net_ptr, const std::string &target_ip,
                    const unsigned short port) {
  port_ = port;
  target_ip_ = target_ip;
  net_ptr_ = net_ptr;
}

void Ptp::ReceivePtpData(const nlohmann::json &data) {
  const std::string fun = data["f"];
  if (fun == "a") {
    time_t1_ = data["a"];
    time_t2_ = data["b"];
    updated_t1_t2_ = true;
  }
  if (fun == "b") {
    const uint64_t t3 = data["a"];
    const uint64_t t4 =
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    if (updated_t1_t2_) {
      // 发送时间偏差到从设备
      int64_t delay = static_cast<int64_t>(t4 - t3 + time_t2_ - time_t1_) / 2;
      int64_t offset = static_cast<int64_t>(time_t2_ - time_t1_ - t4 + t3) / 2;
      const nlohmann::json jsons = {
          {"f", "b"},
          {"a", delay},
          {"b", offset},
      };
      std::string out = jsons.dump() + "\n";
      if (net_ptr_ != nullptr) {
        net_ptr_->sendTo(out.data(), out.size(), target_ip_, port_);
        updated_t1_t2_ = false;
      } else if (serial_ptr_ != nullptr) {
        serial_ptr_->write(reinterpret_cast<uint8_t *>(out.data()), out.size());
        updated_t1_t2_ = false;
      }
      // std::cout << "delay: " << delay << " offset: " << offset << std::endl;
    }
  }
}

void Ptp::SendPtpData() const {
  uint64_t mark =
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
          .count();
  const nlohmann::json data = {
      {"f", "a"},
      {"a", mark},
  };
  std::string out = data.dump() + "\n";
  if (net_ptr_ != nullptr) {
    net_ptr_->sendTo(out.data(), out.size(), target_ip_, port_);
  } else if (serial_ptr_ != nullptr) {
    serial_ptr_->write(reinterpret_cast<const uint8_t *>(out.data()), out.size());
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

}  // namespace infinite_sense