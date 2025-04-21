#include "infinite_sense.h"
#include "json.hpp"
#include "net_manager.h"
#include "serial_manager.h"
#include "log.h"

namespace infinite_sense {


Synchronizer::Synchronizer(std::string config) {

// 输出信息
LOG(INFO) << "\n"
         <<  "  ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▖▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖▗▄▄▄▖" << "\n"
         <<  "    █  ▐▛▚▖▐▌▐▌     █  ▐▛▚▖▐▌  █    █  ▐▌   ▐▌   ▐▌   ▐▛▚▖▐▌▐▌   ▐▌   " << "\n"
         <<  "    █  ▐▌ ▝▜▌▐▛▀▀▘  █  ▐▌ ▝▜▌  █    █  ▐▛▀▀▘ ▝▀▚▖▐▛▀▀▘▐▌ ▝▜▌ ▝▀▚▖▐▛▀▀▘" << "\n"
         <<  "  ▗▄█▄▖▐▌  ▐▌▐▌   ▗▄█▄▖▐▌  ▐▌▗▄█▄▖  █  ▐▙▄▄▖▗▄▄▞▘▐▙▄▄▖▐▌  ▐▌▗▄▄▞▘▐▙▄▄▖";

  // 读取json配置文件 
  readConfigFile(config);
  SetLogPath(config_ptr_->log_dir);
  // 根据配置文件设置参数
  if (config_ptr_->communication_type == "usb") {
    serial_manager_ = std::make_shared<SerialManager>(config_ptr_->device, config_ptr_->usb_baud_rate, data_ptr_);
  } else if (config_ptr_->communication_type == "udp") {
    net_manager_ = std::make_shared<NetManager>(config_ptr_->udp_ip, config_ptr_->udp_port, data_ptr_);
  }
};

void Synchronizer::readConfigFile(const std::string path) {
  config_ptr_ = std::make_shared<Parameters>();
  // Read the JSON file
  std::ifstream file(path);
  if (!file.is_open()) {
      throw std::runtime_error("Could not open config file: " + path);
  }
  nlohmann::json config;
  try {
    file >> config;
  } catch (const nlohmann::json::parse_error& e) {
    throw std::runtime_error("JSON parse error: " + std::string(e.what()));
  }

  // Parse the configuration
  try {
    // SDK section
    if (config.contains("sdk") && config["sdk"].contains("log_dir")) {
        config_ptr_->log_dir = config["sdk"]["log_dir"].get<std::string>();
    }

    // Time base
    if (config.contains("time_base")) {
        config_ptr_->time_base = config["time_base"].get<std::string>();
    }

    // Communication section
    if (config.contains("communication")) {
        const auto& comm = config["communication"];
        if (comm.contains("type")) {
            config_ptr_->communication_type = comm["type"].get<std::string>();
        }

        // udp settings
        if (comm.contains("udp")) {
            const auto& udp = comm["udp"];
            if (udp.contains("port")) {
                config_ptr_->udp_port = udp["port"].get<int>();
            }
            if (udp.contains("ip")) {
                auto ip_array = udp["ip"].get<std::vector<int>>();
                config_ptr_->udp_ip = "[" + std::to_string(ip_array[0]) + "," +
                                std::to_string(ip_array[1]) + "," +
                                std::to_string(ip_array[2]) + "," +
                                std::to_string(ip_array[3]) + "]";
            }
            if (udp.contains("subnet")) {
                auto subnet_array = udp["subnet"].get<std::vector<int>>();
                config_ptr_->udp_subnet = "[" + std::to_string(subnet_array[0]) + "," +
                                   std::to_string(subnet_array[1]) + "," +
                                   std::to_string(subnet_array[2]) + "," +
                                   std::to_string(subnet_array[3]) + "]";
            }
        }

        // USB settings
        if (comm.contains("usb")) {
            const auto& usb = comm["usb"];
            if (usb.contains("device")) {
                config_ptr_->device = usb["device"].get<std::string>();
            }
            if (usb.contains("baud_rate")) {
                config_ptr_->usb_baud_rate = usb["baud_rate"].get<int>();
            }
        }
    }

    // IO section
    if (config.contains("IO")) {
        const auto& io = config["IO"];

        // Camera settings
        if (io.contains("camera")) {
            const auto& camera = io["camera"];
            if (camera.contains("hz")) {
                config_ptr_->io_camera_hz = camera["hz"].get<int>();
            }
            if (camera.contains("reverse")) {
                config_ptr_->io_camera_reverse = camera["reverse"].get<bool>();
            }
            if (camera.contains("duration")) {
                config_ptr_->io_camera_duration = camera["duration"].get<int>();
            }
        }

        // Device settings
        auto parse_device = [&](const std::string& device_name, 
                               std::string& mode, int& hz, bool& reverse, 
                               int& duration, int& max_frequency) {
            if (io.contains(device_name)) {
                const auto& device = io[device_name];
                if (device.contains("mode")) {
                    mode = device["mode"].get<std::string>();
                }
                if (device.contains("hz")) {
                    hz = device["hz"].get<int>();
                }
                if (device.contains("reverse")) {
                    reverse = device["reverse"].get<bool>();
                }
                if (device.contains("duration")) {
                    duration = device["duration"].get<int>();
                }
                if (device.contains("max_frequency")) {
                    max_frequency = device["max_frequency"].get<int>();
                }
            }
        };

        parse_device("device_1", config_ptr_->io_device_mode_1, config_ptr_->io_device_hz_1, 
                    config_ptr_->io_device_reverse_1, config_ptr_->io_device_duration_1, 
                    config_ptr_->io_device_max_frequency_1);
        parse_device("device_2", config_ptr_->io_device_mode_2, config_ptr_->io_device_hz_2, 
                    config_ptr_->io_device_reverse_2, config_ptr_->io_device_duration_2, 
                    config_ptr_->io_device_max_frequency_2);
        parse_device("device_3", config_ptr_->io_device_mode_3, config_ptr_->io_device_hz_3, 
                    config_ptr_->io_device_reverse_3, config_ptr_->io_device_duration_3, 
                    config_ptr_->io_device_max_frequency_3);
    }

    // PPS settings
    if (config.contains("PPS")) {
        const auto& pps = config["PPS"];
        if (pps.contains("baud_rate")) {
            config_ptr_->pps_baud_rate = pps["baud_rate"].get<int>();
        }
        if (pps.contains("flow_control")) {
            config_ptr_->pps_flow_control = pps["flow_control"].get<bool>();
        }
        if (pps.contains("data_bits")) {
            config_ptr_->pps_data_bits = pps["data_bits"].get<int>();
        }
        if (pps.contains("stop_bits")) {
            config_ptr_->pps_stop_bits = pps["stop_bits"].get<int>();
        }
        if (pps.contains("parity")) {
            config_ptr_->pps_parity = pps["parity"].get<int>();
        }
    }

    // GPS settings
    if (config.contains("GPS")) {
        const auto& gps = config["GPS"];
        if (gps.contains("baud_rate")) {
            config_ptr_->gps_baud_rate = gps["baud_rate"].get<int>();
        }
        if (gps.contains("flow_control")) {
            config_ptr_->gps_flow_control = gps["flow_control"].get<bool>();
        }
        if (gps.contains("data_bits")) {
            config_ptr_->gps_data_bits = gps["data_bits"].get<int>();
        }
        if (gps.contains("stop_bits")) {
            config_ptr_->gps_stop_bits = gps["stop_bits"].get<int>();
        }
        if (gps.contains("parity")) {
            config_ptr_->gps_parity = gps["parity"].get<int>();
        }
    }

  } catch (const nlohmann::json::exception& e) {
    throw std::runtime_error("Error parsing config: " + std::string(e.what()));
  }
}


void Synchronizer::SetLogPath(const std::string& path) { SetLogDestination(FATAL, path.c_str()); }

void Synchronizer::Start() const {
  if (net_manager_) {
    net_manager_->Start();
  }
  if (serial_manager_) {
    serial_manager_->Start();
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
  LOG(INFO) << "Synchronizer stopped";
}

void Synchronizer::PrintSummary() {
  std::ostringstream oss;
    
  oss << "\n===== System Parameters =====\n";
  
  // General settings
  oss << "General Settings:\n";
  oss << "  log_dir: " << config_ptr_->log_dir << "\n";
  oss << "  time_base: " << config_ptr_->time_base << "\n";
  oss << "  communication_type: " << config_ptr_->communication_type << "\n\n";
  
  // udp settings
  oss << "udp Settings:\n";
  oss << "  port: " << config_ptr_->udp_port << "\n";
  oss << "  ip: " << config_ptr_->udp_ip << "\n";
  oss << "  subnet: " << config_ptr_->udp_subnet << "\n\n";
  
  // USB settings
  oss << "USB Settings:\n";
  oss << "device: " << config_ptr_->device << "\n";
  oss << "  baud_rate: " << config_ptr_->usb_baud_rate << "\n";
  
  // IO Camera settings
  oss << "IO Camera Settings:\n";
  oss << "  hz: " << config_ptr_->io_camera_hz << "\n";
  oss << "  reverse: " << std::boolalpha << config_ptr_->io_camera_reverse << "\n";
  oss << "  duration: " << config_ptr_->io_camera_duration << "\n\n";
  
  // IO Device settings
  auto logDevice = [&oss](const std::string& name, 
                        const std::string& mode, int hz, bool reverse, 
                        int duration, int max_freq) {
      oss << name << " Settings:\n";
      oss << "  mode: " << mode << "\n";
      oss << "  hz: " << hz << "\n";
      oss << "  reverse: " << std::boolalpha << reverse << "\n";
      oss << "  duration: " << duration << "\n";
      oss << "  max_frequency: " << max_freq << "\n\n";
  };
  
  logDevice("IO Device 1", config_ptr_->io_device_mode_1, config_ptr_->io_device_hz_1, 
           config_ptr_->io_device_reverse_1, config_ptr_->io_device_duration_1, 
           config_ptr_->io_device_max_frequency_1);
  logDevice("IO Device 2", config_ptr_->io_device_mode_2, config_ptr_->io_device_hz_2, 
           config_ptr_->io_device_reverse_2, config_ptr_->io_device_duration_2, 
           config_ptr_->io_device_max_frequency_2);
  logDevice("IO Device 3", config_ptr_->io_device_mode_3, config_ptr_->io_device_hz_3, 
           config_ptr_->io_device_reverse_3, config_ptr_->io_device_duration_3, 
           config_ptr_->io_device_max_frequency_3);
  
  // PPS settings
  oss << "PPS Settings:\n";
  oss << "  baud_rate: " << config_ptr_->pps_baud_rate << "\n";
  oss << "  flow_control: " << std::boolalpha << config_ptr_->pps_flow_control << "\n";
  oss << "  data_bits: " << config_ptr_->pps_data_bits << "\n";
  oss << "  stop_bits: " << config_ptr_->pps_stop_bits << "\n";
  oss << "  parity: " << config_ptr_->pps_parity << "\n\n";
  
  // GPS settings
  oss << "GPS Settings:\n";
  oss << "  baud_rate: " << config_ptr_->gps_baud_rate << "\n";
  oss << "  flow_control: " << std::boolalpha << config_ptr_->gps_flow_control << "\n";
  oss << "  data_bits: " << config_ptr_->gps_data_bits << "\n";
  oss << "  stop_bits: " << config_ptr_->gps_stop_bits << "\n";
  oss << "  parity: " << config_ptr_->gps_parity << "\n";
  
  oss << "===========================\n";
  
  LOG(INFO) << oss.str();
}
}  // namespace infinite_sense
