#include "ptp.h"
#include "usb.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace infinite_sense {

namespace {
constexpr char FUNC_NAME[] = "f";
constexpr char FUNC_TYPE_A[] = "a";
constexpr char FUNC_TYPE_B[] = "b";
}  // namespace

void Ptp::SetUsbPtr(const std::shared_ptr<serial::Serial>& serial_ptr) {
  serial_ptr_ = serial_ptr;
}

void Ptp::SetNetPtr(const std::shared_ptr<UDPSocket>& net_ptr, const std::string& target_ip,
                    unsigned short port) {
  net_ptr_ = net_ptr;
  target_ip_ = target_ip;
  port_ = port;
}

void Ptp::ReceivePtpData(const nlohmann::json& data) {
  try {
    const std::string func = data.at(FUNC_NAME);

    if (func == FUNC_TYPE_A) {
      HandleTimeSyncRequest(data);
    } else if (func == FUNC_TYPE_B) {
      HandleTimeSyncResponse(data);
    }
  } catch (const nlohmann::json::exception& e) {
    LOG(ERROR) << "PTP JSON parse error: " << e.what();
  }
}

void Ptp::HandleTimeSyncRequest(const nlohmann::json& data) {
  try {
    time_t1_ = data.at(FUNC_TYPE_A);
    time_t2_ = data.at(FUNC_TYPE_B);
    updated_t1_t2_ = true;
  } catch (const nlohmann::json::exception& e) {
    LOG(ERROR) << "Invalid 'a' message format: " << e.what();
  }
}

void Ptp::HandleTimeSyncResponse(const nlohmann::json& data) {
  try {
    const uint64_t t3 = data.at(FUNC_TYPE_A);
    const uint64_t t4 = GetCurrentTimeUs();

    if (updated_t1_t2_) {
      const int64_t delay = static_cast<int64_t>(t4 - t3 + time_t2_ - time_t1_) / 2;
      const int64_t offset = static_cast<int64_t>(time_t2_ - time_t1_ - t4 + t3) / 2;

      nlohmann::json response = {
          {FUNC_NAME, FUNC_TYPE_B},
          {FUNC_TYPE_A, delay},
          {FUNC_TYPE_B, offset},
      };

      SendJson(response);
      updated_t1_t2_ = false;
    }
  } catch (const nlohmann::json::exception& e) {
    LOG(ERROR) << "Invalid 'b' message format: " << e.what();
  }
}

void Ptp::SendPtpData() const {
  const uint64_t mark = GetCurrentTimeUs();

  nlohmann::json data = {
      {FUNC_NAME, FUNC_TYPE_A},
      {FUNC_TYPE_A, mark},
  };

  SendJson(data);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Ptp::SendJson(const nlohmann::json& data) const {
  const std::string out = data.dump() + "\n";

  if (net_ptr_) {
    net_ptr_->sendTo(out.data(), out.size(), target_ip_, port_);
  } else if (serial_ptr_) {
    serial_ptr_->write(reinterpret_cast<const uint8_t*>(out.data()), out.size());
  } else {
    LOG(WARNING) << "No valid output interface for PTP message.";
  }
}

uint64_t Ptp::GetCurrentTimeUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

}  // namespace infinite_sense
