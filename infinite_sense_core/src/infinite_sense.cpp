#include "infinite_sense.h"

#include "trigger.h"
#include "net.h"
#include "usb.h"
#include "../../example/GigeCam/mv_cam.h"
#include "messenger.h"
#include "log.h"

namespace infinite_sense {
Synchronizer::Synchronizer() {
LOG(INFO) << "\n"
         <<  "  ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▖▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖▗▄▄▄▖" << "\n"
         <<  "    █  ▐▛▚▖▐▌▐▌     █  ▐▛▚▖▐▌  █    █  ▐▌   ▐▌   ▐▌   ▐▛▚▖▐▌▐▌   ▐▌   " << "\n"
         <<  "    █  ▐▌ ▝▜▌▐▛▀▀▘  █  ▐▌ ▝▜▌  █    █  ▐▛▀▀▘ ▝▀▚▖▐▛▀▀▘▐▌ ▝▜▌ ▝▀▚▖▐▛▀▀▘" << "\n"
         <<  "  ▗▄█▄▖▐▌  ▐▌▐▌   ▗▄█▄▖▐▌  ▐▌▗▄█▄▖  █  ▐▙▄▄▖▗▄▄▞▘▐▙▄▄▖▐▌  ▐▌▗▄▄▞▘▐▙▄▄▖";
};
void Synchronizer::SetLogPath(const std::string& path) { SetLogDestination(FATAL, path.c_str()); }
void Synchronizer::SetNetLink(std::string net_dev, const unsigned int port) {
  net_ip_ = std::move(net_dev);
  net_port_ = port;
  net_manager_ = std::make_shared<NetManager>(net_ip_, net_port_);
}
void Synchronizer::SetUsbLink(std::string serial_dev, const int serial_baud_rate) {
  serial_dev_ = std::move(serial_dev);
  serial_baud_rate_ = serial_baud_rate;
  serial_manager_ = std::make_shared<UsbManager>(serial_dev_, serial_baud_rate_);
  net_manager_ = nullptr;
}
void Synchronizer::UseCam(const std::shared_ptr<Sensor> &cam) {
  cam_manager_ = cam;
}
bool Synchronizer::GetLastTriggerTime(const TriggerDevice dev, uint64_t time) {
  return GET_LAST_TRIGGER_STATUS(dev,time);
}
void Synchronizer::Start() const {
  if (net_manager_) {
    net_manager_->Start();
  }
  if (serial_manager_) {
    serial_manager_->Start();
  }
  if (cam_manager_) {
    std::this_thread::sleep_for(std::chrono::milliseconds{2000});
    cam_manager_->Initialization();
    cam_manager_->Start();
  }
  LOG(INFO) << "Synchronizer started";
}
void Synchronizer::Stop() const {
  if (net_manager_) {
    net_manager_->Stop();
  }
  if (serial_manager_) {
    serial_manager_->Stop();
  }
  if (cam_manager_) {
    cam_manager_->Stop();
  }
  LOG(INFO) << "Synchronizer stopped";
}

void Synchronizer::PrintSummary() {
  TopicMonitor::GetInstance().Start();
  std::this_thread::sleep_for(std::chrono::milliseconds{1001});
  TopicMonitor::GetInstance().Stop();
  LOG(INFO) << TopicMonitor::GetInstance();
}
}  // namespace infinite_sense
