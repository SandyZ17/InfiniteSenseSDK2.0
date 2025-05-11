# 如何查看同步板上传数据
如果是USB连接:则需要安装串口工具，如：cutecom。
```bash
sudo apt-get install cutecom # 下载(可选)
sudo cutecom                 # 启动
```
<p align="center">
<img  style="width:50%;"  alt="cutecom" src="../picture/cutecom.png">
</p>

如果是网口连接： 则使用nc指令，Linux中nc命令是一个功能强大的网络工具，全称是netcat

```bash
# 192.168.1.188 8888 表示同步板的IP地址和端口
echo “Hello InfiniteSense” | nc -u 192.168.1.188 8888
```
<p align="center">
<img  style="width:50%;"  alt="nc" src="../picture/nc.png">
</p>

---
# IMU Data
```json
{
  "f": "imu",            # 数据类型
  "t": 1745767254869878, # 时间戳
  "c": 920480,           # 采集帧数
  "d": [                 # IMU原始数据
    0.416606,            # 加速度计x轴
    -0.181966,           # 加速度计y轴
    9.993754,            # 加速度计z轴
    0.003141,            # 陀螺仪x轴
    0.001038,            # 陀螺仪y轴
    0.000016,            # 陀螺仪z轴
    28.50241             # 温度
  ],
  "q": [                 # 自带IMU计算的姿态
    0.999765,            # 四元数w
    -0.009251,           # 四元数x
    -0.021048,           # 四元数y
    -0.013569            # 四元数z
  ]
}
```
# GPS Data

# Sync Time Data
```json
{
    "f": "b",
    "a": 1740815274,
    "b": 1740815274
}

```

# Device trigger status
```json
{
  "f": "t",                     # 数据类型
  "s": 32,                      # (uint8_t)状态位
  "t": 1745767200873878,        # 时间戳
  "c": 893482                   # 采集帧数
} 
```
