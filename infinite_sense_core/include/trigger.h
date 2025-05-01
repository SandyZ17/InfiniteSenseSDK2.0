#pragma once

#include <mutex>
#include <infinite_sense.h>
#include <map>
namespace infinite_sense {
#define SET_LAST_TRIGGER_STATUS(timestamp, status) TriggerManger::GetInstance().SetLastTriggerStatus(timestamp, status)
#define GET_LAST_TRIGGER_STATUS(device, timestamp) TriggerManger::GetInstance().GetLastTriggerStatus(device, timestamp)
class TriggerManger {
 public:
  static TriggerManger &GetInstance() {
    static TriggerManger instance;
    return instance;
  }
  TriggerManger(const TriggerManger &) = delete;
  TriggerManger &operator=(const TriggerManger &) = delete;
  void SetLastTriggerStatus(const uint64_t &, const uint8_t &);
  bool GetLastTriggerStatus(TriggerDevice, uint64_t &);
 private:
  static bool GetBool(const uint8_t data, const int index) { return (data >> index) & 1; }
  void UpdateAndPublishDevice(TriggerDevice dev, int bit_index, uint64_t time);
  void PublishDeviceStatus(TriggerDevice dev, uint64_t time, bool status);
  TriggerManger();
  ~TriggerManger() = default;
  uint8_t status_{0};
  std::mutex lock_{};
  std::map<TriggerDevice, std::tuple<bool, uint64_t>> status_map_{};
  std::map<TriggerDevice, std::string> device_topics_; // 设备-topic映射
};
}  // namespace infinite_sense
