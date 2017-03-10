/*
 * camera_topic.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: sajjad_rahnama
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
  VideoCapture cap("http://192.168.43.92:8080/?action=stream.mjpg");
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
