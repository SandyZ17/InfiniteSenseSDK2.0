
#include "ser.h"
#include "infinite_sense.h"
#include "ptp.h"
#include "sensor.h"
namespace infinite_sense {
SerialManager::SerialManager(const std::string &port, const int baud_rate) {
  port_ = port;
  serial_ptr_ = std::make_unique<serial::Serial>();
  try {
    serial_ptr_->setPort(port_);
    serial_ptr_->setBaudrate(baud_rate);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    serial_ptr_->setTimeout(to);
    serial_ptr_->open();
  } catch (serial::IOException &e) {
    LOG(ERROR) << "Unable to open serial port: " << serial_ptr_->getPort() << ", " << e.what();
    if (serial_ptr_->isOpen()) {
      serial_ptr_->close();
    }
    return;
  }
  if (serial_ptr_->isOpen()) {
    serial_ptr_->flush();
    LOG(ERROR) << "Serial port: " << serial_ptr_->getPort() << " initialized and opened.";
  }
  ptp_ = std::make_unique<Ptp>();
  ptp_->SetSerialPtr(serial_ptr_);
}

SerialManager::~SerialManager() {
  if (serial_ptr_->isOpen()) {
    serial_ptr_->close();
    LOG(ERROR) << "Serial port: " << serial_ptr_->getPort() << " closed.";
  }
  rx_thread_.join();
  tx_thread_.join();
}

void SerialManager::Start() {
  started_ = true;
  rx_thread_ = std::thread([this] { Receive(); });
  tx_thread_ = std::thread([this] { TimeStampSynchronization(); });
  LOG(INFO) << "Serial manager started";
}

void SerialManager::Stop() {
  started_ = false;
  if (serial_ptr_->isOpen()) {
    serial_ptr_->close();
    LOG(WARNING) << "Serial port: " << serial_ptr_->getPort() << " closed.";
  }
  rx_thread_.join();
  tx_thread_.join();
  LOG(INFO) << "Serial manager stopped";
}
void SerialManager::Receive() const {
  while (started_) {
    if (serial_ptr_->available()) {
      std::string serial_recv = serial_ptr_->readline();
      static bool ignore = true;
      if (serial_recv.empty() || ignore) {
        ignore = false;
        continue;
      }
      try {
        auto json_data = nlohmann::json::parse(serial_recv);
        if (json_data.empty()) {
          continue;
        }
        ptp_->ReceivePtpData(json_data);
        ProcessTriggerData(json_data);
        ProcessIMUData(json_data);
        ProcessGPSData(json_data);
        ProcessLOGData(json_data);
      } catch (const nlohmann::json::parse_error &e) {
        LOG(ERROR) << "Failed to parse JSON: " << e.what();
        LOG(ERROR) << "Received data: " << serial_recv;
      }
    }
    std::this_thread::sleep_for(std::chrono::microseconds{10});
  }
}
void SerialManager::TimeStampSynchronization() const {
  while (started_) {
    ptp_->SendPtpData();
  }
}
}  // namespace infinite_sense