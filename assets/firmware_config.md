# 固件配置

## 第三代同步板
固件支持网口和串口配置，配置完成后自动重启。建议采用任意串口工具发送以下指令：
```aiignore
{"f":"cfg","port":8888,"ip":[192,168,1,188],"subnet":[255,255,255,0],"hz_cam_1":20,"hz_cam_2":30,"hz_cam_3":40,"hz_cam_4":50,"uart_0_baud_rate":921600,"uart_1_baud_rate":9600,"uart_2_baud_rate":115200,"use_gps":true,"use_pps":true,"version":300}
```
## 第四代同步板
固件支持网口和串口配置，配置完成后自动重启。建议采用任意串口工具发送以下指令：
```aiignore
{"f":"cfg","port":8888,"ip":[192,168,1,188],"subnet":[255,255,255,0],"hz_cam_1":20,"hz_cam_2":30,"hz_cam_3":40,"hz_cam_4":50,"uart_0_baud_rate":921600,"uart_1_baud_rate":9600,"uart_2_baud_rate":115200,"use_gps":true,"use_pps":true,"version":400}
```

配置指令说明：
```python
  "f": "cfg",                 // 配置文件类型标识符（固定为"cfg"）
  "port": 8888,               // 网络通信端口号
  "ip": [192,168,1,188],      // 设备IP地址（IPv4四元组格式）
  "subnet": [255,255,255,0],  // 子网掩码配置
  "hz_cam_1": 20,             // 相机1触发频率（单位：Hz，建议≤100Hz）
  "hz_cam_2": 30,             // 相机2触发频率（单位：Hz，建议≤100Hz）
  "hz_cam_3": 40,             // 相机3触发频率（单位：Hz，建议≤100Hz）
  "hz_cam_4": 50,             // 相机4触发频率（单位：Hz，建议≤100Hz）
  "uart_0_baud_rate": 921600, // 通讯波特率（高速模式，用于时间同步/传感器数据）
  "uart_1_baud_rate": 9600,   // PPS波特率（低速模式，用于雷达同步）
  "uart_2_baud_rate": 115200, // GPS波特率（中速模式，用于GPS数据读取）
  "use_gps": true,            // GPS模块启用标志
  "use_pps": true             // PPS精确时钟同步信号启用标志
  "version": 400              // 固件版本号(V3/MINI：300，V4：400)
```
