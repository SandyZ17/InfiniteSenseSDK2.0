#pragma once
#include "log.h"
#include "data.h"
#include "messenger.h"

namespace infinite_sense {

class NetManager;
class SerialManager;
class CamManger;
class TriggerManger;
class Messenger;
class Synchronizer {
 public:
  Synchronizer();
  ~Synchronizer() = default;
  void Start() const;
  void Stop() const;
  static void SetLogPath(const std::string &path);
  void SetSerialLink(std::string serial_dev, int serial_baud_rate);
  void SetNetLink(std::string net_dev, unsigned int port);
  void UseMvCam(const std::map<std::string, TriggerDevice>& params = std::map<std::string, TriggerDevice>());
  static bool GetLastTriggerTime(TriggerDevice dev, uint64_t time);
  static void PrintSummary() ;
 private:
  std::string net_ip_;
  unsigned short net_port_{};
  std::string serial_dev_;
  int serial_baud_rate_{};
  std::shared_ptr<NetManager> net_manager_{nullptr};
  std::shared_ptr<SerialManager> serial_manager_{nullptr};
  std::shared_ptr<CamManger> cam_manager_{nullptr};
};

}  // namespace infinite_sense
