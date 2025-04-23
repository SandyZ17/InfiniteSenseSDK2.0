<p align="center">
<img  style="width:50%;"  alt="Logo" src="assets/main_logo.png">
<br>
<em>稳定 易用 精度</em>
<br>
</p>
<p align="center">
<a href="README.md">English</a>
</p>

---

# 🚀 SDK 全新升级：更强大，更易用！

这次升级为 SDK 带来了更流畅的开发体验和更强大的功能：

✨ 精简依赖 – 降低编译开销，构建更快速。  
🤖 支持 ROS2 & Python – 轻松集成现代机器人与脚本化工作流。  
⏱ 更精准的同步机制 – 提供更高精度的时间协调。  
📡 数据协议更透明 – 通信更清晰、更灵活。  
⚙️ 配置更简单 – 轻松上手，自定义更便捷。  
📜 日志功能增强 – 记录更全面，调试更高效。  

立即升级，体验更高效、更友好的 SDK！🚀

# 介绍
  SDK 2.0 为前代重大升级版，新增多相机/雷达/IMU及单GPS混合信号协同管理能力，强化复杂传感器组网性能。采用ZeroMQ实现跨平台毫秒级数据同步，并通过开源核心代码与标准化接口，支持嵌入式/桌面/云端多场景灵活部署。

## 目前可公开的情报

### 2.0 Demo版本更新

1. 使用JSON作为数据交互协议
2. V3-Full硬件和Mini硬件支持
3. 多路自定义频率触发(V4硬件)

### 更加准确的触发对齐与帧率

<p align="center">
<img style="width:50%;" alt="1秒钟触发" src="assets/one_second.png">

<p align="center">
<img  style="width:50%; alt="帧率对齐" src="assets/align.png">

### 📸第四代同步版本

第四代同步板，支持不同设备(2IMU+4CAM+2Lidar)不同频率同步，更加安全的电源。
<p align="center">
<img style="width:50%; alt="board_a" src="assets/board_a.png">

<p align="center">
<img style="width:50%; alt="board_b" src="assets/board_b.png">

---

# 固件配置指南

固件支持网口和串口配置，配置完成后自动重启。串口/网口发送以下信息到设备即可

```aiignore
{"f":"cfg","port":8888,"ip":[192,168,1,188],"subnet":[255,255,255,0],"hz_cam_1":20,"hz_cam_2":30,"hz_cam_3":40,"hz_cam_4":50,"uart_0_baud_rate":921600,"uart_1_baud_rate":9600,"uart_2_baud_rate":115200,"use_gps":true,"use_pps":true}
```
配置指令说明：
```python
{
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
}
```
