#include "infinite_sense.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/image_encodings.hpp"
#include "image_transport/image_transport.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
class InfiniteSenseGigeDriver : public rclcpp::Node {
 public:
  InfiniteSenseGigeDriver()
      : Node("gige_driver"),
        node_handle_(),
        transport_(node_handle_),
        img_pub_(transport_.advertise("camera/image_raw", 1)) {}

 private:
  rclcpp::Node::SharedPtr node_handle_;
  image_transport::ImageTransport transport_;
  image_transport::Publisher img_pub_;
};

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::shutdown();

  return 0;
}