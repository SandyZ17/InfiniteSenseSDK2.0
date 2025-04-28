
# 第四代同步板(V4)

(可选)串口工具安装：

```bash
sudo apt-get install cutecom
```

固件支持网口和串口配置，配置完成后自动重启并加载最新配置。打开任意串口工具发送以下指令：

```python
{"f":"cfg","port":8888,"ip":[192,168,1,188],"subnet":[255,255,255,0],"hz_cam_1":1,"hz_cam_2":2,"hz_cam_3":4,"hz_cam_4":8,"hz_imu_2":10,"xtal_diff":0,"uart_0_baud_rate":921600,"uart_1_baud_rate":9600,"uart_2_baud_rate":115200,"use_gps":true,"use_pps":true,"version":400}\n
```
对应指令说明：
```python
  "f": "cfg",                 // 配置文件类型标识符（固定为"cfg"）
  "port": 8888,               // 网络通信端口号
  "ip": [192,168,1,188],      // 设备IP地址
  "subnet": [255,255,255,0],  // 子网掩码配置
  "hz_cam_1": 20,             // 相机1触发频率（单位：Hz，建议≤100Hz）
  "hz_cam_2": 30,             // 相机2触发频率（单位：Hz，建议≤100Hz）
  "hz_cam_3": 40,             // 相机3触发频率（单位：Hz，建议≤100Hz）
  "hz_cam_4": 50,             // 相机4触发频率（单位：Hz，建议≤100Hz）
  "hz_imu_2":1                // IMU2触发频率（单位：Hz，建议≤100Hz）
  "uart_0_baud_rate": 921600, // 通讯波特率（高速模式，用于时间同步/传感器数据）
  "uart_1_baud_rate": 9600,   // PPS波特率（低速模式，用于雷达同步）
  "uart_2_baud_rate": 115200, // GPS波特率（中速模式，用于GPS数据读取）
  "use_gps": true,            // GPS模块启用标志
  "use_pps": true,            // PPS精确时钟同步信号启用标志
  "xtal_diff":0,              // 晶振偏差修正
  "version": 400              // 固件版本号(V3/MINI：300，V4：400)
```

# 第三代同步板(V3/MINI)
固件支持网口和串口配置，配置完成后自动重启并加载最新配置。打开任意串口工具发送以下指令：
```python
{"f":"cfg","port":8888,"ip":[192,168,1,188],"subnet":[255,255,255,0],"hz_cam_1":10,"uart_0_baud_rate":921600,"uart_1_baud_rate":9600,"uart_2_baud_rate":115200,"use_gps":true,"use_pps":true,"version":300}\n
```
