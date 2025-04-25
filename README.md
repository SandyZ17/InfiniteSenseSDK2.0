<p align="center">
<img  style="width:50%;"  alt="Logo" src="assets/main_logo.png">
<br>
<em>稳定 易用 精度</em>
<br>
</p>
<p align="center">
<a href="README_EN.md">English</a>
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
# 开始使用
    1. 下载SDK源码
    2. 编译安装
    3. [配置设备](./assets/firmware_config.md)
    3. 运行SDK测试Demo
    4. 阅读SDK文档

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
