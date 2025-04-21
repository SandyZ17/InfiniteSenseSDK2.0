#include "infinite_sense.h"
#include <thread>
#include "mv_camera.h"
using namespace infinite_sense;

// 图像数据读取函数
void ImageReceive(Synchronizer* sync, void *handle, const std::string &name) {
  unsigned int last_count = 0;
  MV_FRAME_OUT st_out_frame;
  CamData cam_data;

  while (true) {
    // 1. 读取相机的数据
    memset(&st_out_frame, 0, sizeof(MV_FRAME_OUT));
    int n_ret = MV_CC_GetImageBuffer(handle, &st_out_frame, 10);
    if (n_ret == MV_OK) {
      // 2. 读取相机的曝光时间
      MVCC_FLOATVALUE expose_time;
      n_ret = MV_CC_GetExposureTime(handle, &expose_time);
      if (n_ret != MV_OK) {
        LOG(ERROR) << "Get ExposureTime fail! n_ret [0x" << std::hex << n_ret << "]";
      }
      // 3. 计算相机触发时间 = IO 触发时间 + 曝光时间 / 2 (也有不考虑曝光时间，直接等于IO触发时间的)
      {
        cam_data.time_stamp_us = sync->GetTriggerData().cam_time_stamp_us + static_cast<uint64_t>(expose_time.fCurValue / 2.);
      }
      // 4. 相机图像数据预处理，格式转换 
      MvGvspPixelType en_dst_pixel_type = PixelType_Gvsp_Undefined;
      unsigned int n_channel_num = 0;
      // 如果是彩色则转成RGB8
      if (IsColor(st_out_frame.stFrameInfo.enPixelType)) {
        n_channel_num = 3;
        en_dst_pixel_type = PixelType_Gvsp_RGB8_Packed;
      }
      // 如果是黑白则转换成Mono8
      else if (IsMono(st_out_frame.stFrameInfo.enPixelType)) {
        n_channel_num = 1;
        en_dst_pixel_type = PixelType_Gvsp_Mono8;
      }
      if (n_channel_num != 0) {
        cam_data.name = name;
        if (en_dst_pixel_type == PixelType_Gvsp_Mono8) {
          cam_data.image = GMat(st_out_frame.stFrameInfo.nHeight, st_out_frame.stFrameInfo.nWidth,
                                GMatType<uint8_t, 1>::Type, st_out_frame.pBufAddr);
        }
        if (en_dst_pixel_type == PixelType_Gvsp_RGB8_Packed) {
          cam_data.image = GMat(st_out_frame.stFrameInfo.nHeight, st_out_frame.stFrameInfo.nWidth,
                                GMatType<uint8_t, 3>::Type, st_out_frame.pBufAddr);
        }

        if (last_count == 0) {
          last_count = st_out_frame.stFrameInfo.nFrameNum;
        } else {
          last_count++;
          if (last_count != st_out_frame.stFrameInfo.nFrameNum) {
            LOG(WARNING) << "Loss of data from cam connection : " << last_count;
            last_count = 0;
          }
        }
      }
    }
    // 读取得到图像 cam_data 格式，输出信息

    LOG(INFO) << "Read Image name:"<< cam_data.name<<" timesstampe: " <<  cam_data.time_stamp_us \
    <<" width: " << cam_data.image.GetWidth() <<" height: " << cam_data.image.GetHeight() << std::endl;

    // 清空内存 
    if (nullptr != st_out_frame.pBufAddr) {
      n_ret = MV_CC_FreeImageBuffer(handle, &st_out_frame);
      if (n_ret != MV_OK) {
        LOG(ERROR) << "Free Image Buffer fail! n_ret [0x" << std::hex << n_ret << "]";
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds{2});
  }
}


int main(int argc, char **argv)
{
  if (argc < 2){
    std::cerr << "Usage: ./infinite_sense_node <config_file>" << std::endl;
  }
  // 读取配置文件
  std::string config_path = argv[1];
  std::cout<<"Read config from "<<config_path<<std::endl;
  Synchronizer synchronizer(config_path);
  // 打印配置信息
  synchronizer.PrintSummary();

  // PC 与 同步板通信线程
  synchronizer.Start();

  // 相机设备读取线程
  MVCamera cameras;
  // 获取相机数量，每个相机新建一个线程
  std::vector<std::thread> cam_threads;
  for (int i = 0;i < cameras.DeviceNum(); i++) {
    // 海康相机驱动的一些操作 
    void* handle = cameras.GetHandle(i); // 获取第i个相机
    int n_ret = MV_OK;
    MVCC_STRINGVALUE pst_value;
    n_ret = MV_CC_GetDeviceUserID(handle, &pst_value);// 获取用户配置的id
    if (n_ret != MV_OK) {
      LOG(ERROR) << "Get DeviceUserID fail! n_ret [0x" << std::hex << n_ret << "]";
    }
    // 用户配置的相机名称，可以用来区分第几个相机
    auto name = std::string(pst_value.chCurValue);
    if (name.empty()) {
      static int cam_index{0};
      name = "cam_" + std::to_string(cam_index++);
      LOG(WARNING) << "Camera name is empty,create name " << name;
    }
    // 创建新的线程,调用ImageReceiv处理
    cam_threads.emplace_back(&ImageReceive,  &synchronizer, handle, name);
    LOG(INFO) << "Camera name is " << name << " start";
  }

  // 输出读取的当前数据
  while (true) {
    if (synchronizer.ImuUpdate()) {
        ImuData imu = synchronizer.GetImuData();
        LOG(INFO) << "ImuData: {" 
        << " time_stamp_us: " << imu.time_stamp_us
        << ", temperature: " << imu.temperature
        << ", name: " << imu.name
        << ", a: [" << imu.a[0] << ", " << imu.a[1] << ", " << imu.a[2] << "]"
        << ", g: [" << imu.g[0] << ", " << imu.g[1] << ", " << imu.g[2] << "]"
        << ", q: [" << imu.q[0] << ", " << imu.q[1] << ", " << imu.q[2] << ", " << imu.q[3] << "]"
        << " }";
    }
    
    GPSData gps = synchronizer.GetGPSData();
    LOG(INFO) << "GPSData: {"
    << " time_stamp_us: " << gps.time_stamp_us
    << ", gps_stamp_us: " << gps.gps_stamp_us
    << ", gps_stamp_us_trigger: " << gps.gps_stamp_us_trigger
    << ", name: \"" << gps.name << "\""
    << ", latitude: " << gps.latitude
    << ", longitude: " << gps.longitude
    << " }";

    TriggerData trigger = synchronizer.GetTriggerData();
    LOG(INFO) << "TriggerData: {"
    << " cam_time_stamp_us: " << trigger.cam_time_stamp_us
    << ", dev_1_time_stamp_us: " << trigger.dev_1_time_stamp_us
    << ", dev_2_time_stamp_us: " << trigger.dev_2_time_stamp_us
    << ", dev_3_time_stamp_us: " << trigger.dev_3_time_stamp_us
    << " }";
    
    std::this_thread::sleep_for(std::chrono::milliseconds{250});
  }

  synchronizer.Stop();
  return 0;
}
