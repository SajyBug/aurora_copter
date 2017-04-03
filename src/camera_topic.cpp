/* 
 * @File:     camera_topic.cpp
 * @Author:   Sajjad Rahnama
 * 
 * @Project:  Aurora
 * @Version:  1.0 - Iran Open 2017
 * 
 * @Created Oct 12, 2016
 */

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char** argv)
{

  ros::init(argc, argv, "camera_topic");
  Mat frame;
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_(nh_);
  image_transport::Publisher image_pub_;
  image_pub_ = it_.advertise("/raspberry_pi/image_raw", 1);
  cv_bridge::CvImage Cvptr;
  Cvptr.encoding = sensor_msgs::image_encodings::BGR8;
VideoCapture cap("http://192.168.7.10:8080/?action=stream.mjpg");
 // VideoCapture cap("http://192.168.43.137:4747/mjpegfeed?320x240.mjpeg");
//VideoCapture cap("http://192.168.7.104:4747/mjpegfeed?320x240.mjpeg");
  while (cap.isOpened())
  {
    cap >> frame;
    Cvptr.image = frame;
    image_pub_.publish(Cvptr.toImageMsg());
    imshow("video", frame);
    if (waitKey(30) >= 0)
    {
      cv::destroyAllWindows();
      break;
    };
  }
  ros::spin();
  return 0;
}
