#include "infinite_sense.h"
#include "mv_cam.h"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/image_encodings.hpp"
#include "image_transport/image_transport.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include <cv_bridge/cv_bridge.h>
class CamDriver final : public rclcpp::Node {
public:
    CamDriver()
        : Node("gige_driver"),
          node_handle_(std::shared_ptr<CamDriver>(this, [](auto *) {})),
          transport_(node_handle_) {
        synchronizer_.SetUsbLink("/dev/ttyACM0", 921600);
        const auto mv_cam = std::make_shared<infinite_sense::MvCam>();
        mv_cam->SetParams({{"cam_1", infinite_sense::CAM_1}});
        synchronizer_.UseSensor(mv_cam);
        synchronizer_.Start();
        imu_pub_ = this->create_publisher<sensor_msgs::msg::Imu>("imu/data", 10);
        img_pub_ = transport_.advertise("image_raw", 10);
        {
            using namespace std::placeholders;
            infinite_sense::Messenger::GetInstance().SubStruct("imu_1", std::bind(&CamDriver::ImuCallback, this, _1, _2));
            infinite_sense::Messenger::GetInstance().SubStruct("cam_1", std::bind(&CamDriver::ImageCallback, this, _1, _2));
        }
    }
    void ImuCallback(const void* msg, size_t) const {
        const auto* imu_data = static_cast<const infinite_sense::ImuData*>(msg);
        sensor_msgs::msg::Imu imu_msg;
        rclcpp::Time stamp(static_cast<uint64_t>(imu_data->time_stamp_us) * 1000); // 微秒 -> 纳秒
        imu_msg.header.stamp = stamp;
        imu_msg.header.frame_id = "imu_link";
        imu_msg.linear_acceleration.x = imu_data->a[0];
        imu_msg.linear_acceleration.y = imu_data->a[1];
        imu_msg.linear_acceleration.z = imu_data->a[2];
        imu_msg.angular_velocity.x = imu_data->g[0];
        imu_msg.angular_velocity.y = imu_data->g[1];
        imu_msg.angular_velocity.z = imu_data->g[2];
        imu_msg.orientation.w = imu_data->q[0];
        imu_msg.orientation.x = imu_data->q[1];
        imu_msg.orientation.y = imu_data->q[2];
        imu_msg.orientation.z = imu_data->q[3];
        imu_pub_->publish(imu_msg);
    }
    void ImageCallback(const void* msg, size_t) {
        const auto* cam_data = static_cast<const infinite_sense::CamData*>(msg);
        // 构造 ROS2 图像消息
        std_msgs::msg::Header header;
        header.stamp = rclcpp::Time(cam_data->time_stamp_us * 1000);  // us -> ns
        header.frame_id = "camera_link";
        const cv::Mat image_mat(cam_data->image.rows, cam_data->image.cols, CV_8UC1, cam_data->image.data);
        sensor_msgs::msg::Image::SharedPtr image_msg =
            cv_bridge::CvImage(header, "mono8", image_mat).toImageMsg();
        img_pub_.publish(image_msg);
    }
private:
    infinite_sense::Synchronizer synchronizer_;
    rclcpp::Node::SharedPtr node_handle_;
    image_transport::ImageTransport transport_;
    image_transport::Publisher img_pub_;
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_;
};

int main(const int argc, char *argv[]) {
    printf("Starting GigeCam ROS2 driver...\n");
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CamDriver>());
    return 0;
}
