<p align="center">
<img style="width:50%;" alt="Logo" src="assets/main_logo.png">
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
## 目前可公开的情报

### 2.0 Demo版本更新
1. 使用JSON作为数据交互协议  
2. V3-Full硬件和Mini硬件支持  
3. 多路自定义频率触发(V4硬件)  

### 更加准确的触发对齐与帧率

<p align="center">
<img style="width:50%;" alt="1秒钟触发" src="assets/one_second.png">
<br>
<p align="center">
<img  style="width:50%; alt="帧率对齐" src="assets/align.png">
<br>

### 📸第四代同步版本
第四代同步板，支持不同设备(2IMU+4CAM+2Lidar)不同频率同步，更加安全的电源。
<p align="center">
<img style="width:50%; alt="board_a" src="assets/board_a.png">
<br>
<p align="center">
<img style="width:50%; alt="board_b" src="assets/board_b.png">
<br>
---

# 固件配置指南
固件支持网口和串口配置，配置完成后自动重启。串口/网口发送以下信息到设备即可
```aiignore
{"f":"cfg","port":8888,"ip":[192,168,1,188],"subnet":[255,255,255,0],"hz_cam_1":20,"hz_dev_1":30,"hz_dev_2":40,"hz_dev_3":50,"uart_0_baud_rate":921600,"uart_1_baud_rate":9600,"uart_2_baud_rate":115200,"use_gps":true,"use_pps":true}
```
