#include "infinite_sense.h"
#include "mv_cam.h"

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Imu.h>

using namespace infinite_sense;
inline ros::Time CreateRosTimestamp(const uint64_t mico_sec) {
  uint32_t nsec_per_second = 1e9;
  auto u64 = mico_sec * 1000;
  uint32_t sec = u64 / nsec_per_second;
  uint32_t nsec = u64 - (sec * nsec_per_second);
  return {sec, nsec};
}

class UdpDemoNode {
 public:
  UdpDemoNode(ros::NodeHandle &nh) : node_(nh), it_(node_), camera_name_("cam_1"), imu_name_("imu_1") {}

  void ImuCallback(const void *msg, size_t) {
    const auto *imu_data = static_cast<const ImuData *>(msg);
    sensor_msgs::Imu imu_msg_data;
    imu_msg_data.header.frame_id = "/base_imu_link";
    imu_msg_data.header.stamp = CreateRosTimestamp(imudata.time_stamp_us);

    imu_msg_data.angular_velocity.x = imu_data->g[0];
    imu_msg_data.angular_velocity.y = imu_data->g[1];
    imu_msg_data.angular_velocity.z = imu_data->g[2];

    imu_msg_data.linear_acceleration.x = imu_data->a[0];
    imu_msg_data.linear_acceleration.y = imu_data->a[1];
    imu_msg_data.linear_acceleration.z = imu_data->a[2];

    imu_msg_data.orientation.w = imu_data->a[0];
    imu_msg_data.orientation.x = imu_data->a[1];
    imu_msg_data.orientation.y = imu_data->a[2];
    imu_msg_data.orientation.z = imu_data->a[3];
    imu_pub_.publish(imu_msg_data);
  }

  // 自定义回调函数
  void ImageCallback(const void *msg, size_t) {
    const auto *cam_data = static_cast<const infinite_sense::CamData *>(msg);
    std_msgs::msg::Header header;
    header.stamp = rclcpp::Time(cam_data->time_stamp_us * 1000);
    header.frame_id = "map";
    const cv::Mat image_mat(cam_data->image.rows, cam_data->image.cols, CV_8UC1, cam_data->image.data);
    const sensor_msgs::msg::Image::SharedPtr image_msg = cv_bridge::CvImage(header, "mono8", image_mat).toImageMsg();
    image_pub_.publish(image_msg);
  }
  void Init() {
    synchronizer_.SetUsbLink("/dev/ttyACM0", 921600);
    mv_cam_ = std::make_shared<MvCam>();
    mv_cam_->SetParams({{"cam_1", CAM_1}});
    synchronizer_.UseSensor(mv_cam_);
    imu_pub_ = node_.advertise<sensor_msgs::Imu>(imu_name_, 1000);
    image_pub_ = it_.advertise(camera_name_, 30);
    synchronizer_.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    Messenger::GetInstance().SubStruct("imu_1", ImuCallback);
    Messenger::GetInstance().SubStruct("cam_1", ImageCallback);
  }

  void Run() {
    ros::Rate loop_rate(1000);
    while (ros::ok()) {
      ros::spinOnce();
      loop_rate.sleep();
    }
    synchronizer_.Stop();
  }

 private:
  ros::NodeHandle &node_;
  image_transport::ImageTransport it_;
  ros::Publisher imu_pub_;
  image_transport::Publisher image_pub_;
  std::shared_ptr<MvCam> mv_cam_;
  Synchronizer synchronizer_;
  std::string camera_name_;
  std::string imu_name_;
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "udp_demo", ros::init_options::NoSigintHandler);
  ros::NodeHandle node;
  UdpDemoNode demo_node(node);
  demo_node.Init();
  demo_node.Run();

  return 0;
}
