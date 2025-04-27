# 系统正常运行打印输出
```python
(串口打开状态)
serial_manager.cpp:25 Serial port: /dev/ttyACM0 initialized and opened.
serial_manager.cpp:44 Serial manager started  ( 使用串口通信 )
infinite_sense.cpp:49 Synchronizer started
messenger.cpp:16 ZMQ PUB: tcp://0.0.0.0:40295 ( 当前使用的ZMQ通讯地址 )
infinite_sense.cpp:68 
--- Topic Monitor ---
  Active Topics (8):             (    活跃话题数量    )
    • imu1 (num: 100)            (1秒内收到收到消息数量 )
    • trigger/imu_1 (num: 100)   (     触发状态      )
    • trigger/cam_4 (num: 8)
    • trigger/cam_3 (num: 4)
    • trigger/cam_2 (num: 2)
    • trigger/laser (num: 1)
    • trigger/cam_1 (num: 1)
    • trigger/imu_2 (num: 1)
---------------------
```