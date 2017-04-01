/* 
 * @File:     vision_opencv_node.cpp
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

static const std::string OPENCV_WINDOW = "Raw Image window";
using namespace cv;

class Edge_Detector
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:

  Edge_Detector() :
    it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/raspberry_pi/image_raw", 1, &Edge_Detector::imageCb, this);
    image_pub_ = it_.advertise("/ardrone/front/image_raw", 1);
    cv::namedWindow(OPENCV_WINDOW);

  }

  ~Edge_Detector()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {

    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    detect_edges(cv_ptr->image);

  }

  void detect_edges(cv::Mat img)
  {
    cv_bridge::CvImage cv_image;
    cv_image.encoding = sensor_msgs::image_encodings::MONO8;
    cv::Mat src_gray;
    //    cv::Mat dst, detected_edges;
    //
    //    int edgeThresh = 1;
    //    int lowThreshold = 200;
    //    int highThreshold =300;
    //    int kernel_size = 5;

    cv::cvtColor(img, src_gray, CV_BGR2GRAY);
    //     cv::blur( src_gray, detected_edges, cv::Size(5,5) );
    //    cv::Canny(src_gray, src_gray, 20, 50);
    cv::resize(src_gray, src_gray, cv::Size(752, 480));
    cv_image.image = src_gray;
    //    dst = cv::Scalar::all(0);
    //    img.copyTo( dst, detected_edges);
    //    dst.copyTo(img);
    image_pub_.publish(cv_image.toImageMsg());
    cv::imshow(OPENCV_WINDOW, src_gray);
    //    cv::imshow(OPENCV_WINDOW_1, src_gray);
    cv::waitKey(3);

  }

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "vision_opencv");
  Edge_Detector ic;
  ros::spin();
  return 0;
}
