#include "infinite_sense.h"

#include "ptp.h"
#include "sensor.h"
#include "net_manager.h"

namespace infinite_sense {
NetManager::NetManager(std::string target_ip, const unsigned short port, std::shared_ptr<SynchronizerData> data)
    : port_(port), target_ip_(std::move(target_ip)) {
  uint64_t curr_time =
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch())
          .count();
  const auto *byte = reinterpret_cast<uint8_t *>(&curr_time);
  net_ptr_ = std::make_shared<UDPSocket>();
  net_ptr_->sendTo(byte, sizeof(uint64_t), target_ip_, port_);
  ptp_ = std::make_unique<Ptp>();
  ptp_->SetNetPtr(net_ptr_, target_ip_, port_);
  data_ = std::move(data);
}

NetManager::~NetManager() {
  net_ptr_->disconnect();
  net_ptr_ = nullptr;
  Stop();
}

void NetManager::Start() {
  started_ = true;
  rx_thread_ = std::thread([this] { Receive(); });
  tx_thread_ = std::thread([this] { TimeStampSynchronization(); });
  net_ptr_->disconnect();
  LOG(INFO) << "Net manager started";
}

void NetManager::Stop() {
  started_ = false;
  rx_thread_.join();
  tx_thread_.join();
  LOG(INFO) << "Net manager stopped";
}
void NetManager::Receive() {
  uint8_t buffer[65540]{};
  string source_address;
  unsigned short source_port;
  while (started_) {
    if (net_ptr_) {
      const int size = net_ptr_->recvFrom(buffer, 65540, source_address, source_port);
      if (size <= 0) {
        continue;
      }
      std::string recv_data(reinterpret_cast<char *>(buffer), size);
      static bool ignor = true;
      if (recv_data.empty() || ignor) {
        ignor = false;
        continue;
      }
      try {
        auto json_data = nlohmann::json::parse(recv_data);
        if (json_data.empty()) {
          continue;
        }
        // LOG(INFO) <<  json_data.dump();
        ptp_->ReceivePtpData(json_data);
        ProcessTriggerData(json_data, trigger_data_);
        bool add = ProcessIMUData(json_data, imu_data_);
        ProcessGPSData(json_data, gps_data_);
        ProcessLOGData(json_data);
        // update
        data_->SetGPSData(gps_data_);
        if (add) {
          data_->AddImuData(imu_data_);
        }
        data_->SetTriggerData(trigger_data_);
      } catch (const nlohmann::json::parse_error &e) {
        LOG(ERROR) << "Failed to parse JSON: " << e.what();
        LOG(ERROR) << "Received data: " << recv_data;
      }
    }
    std::this_thread::sleep_for(std::chrono::microseconds{10});
  }
}
void NetManager::TimeStampSynchronization() const {
  while (started_) {
    ptp_->SendPtpData();
  }
}

}  // namespace infinite_sense