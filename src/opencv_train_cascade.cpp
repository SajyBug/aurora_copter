/* 
 * @File:     opencv_train_cascade.cpp
 * @Author:   Sajjad Rahnama
 * 
 * @Project:  Aurora
 * @Version:  1.0 - Iran Open 2017
 * 
 * @Created Mar 26, 2017
 */
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
static const std::string OPENCV_WINDOW = "Raw Image window";
using namespace cv;

int offset_x = 270;
int offset_y = 190;
int width = 100;
int height = 100;
int number_image = 1;
std::stringstream image_name;
std::string type = ".jpg";
std::string positive_images_path = "/home/sajjadtest/Opencv_Train_Cascade/opencv-haar-classifier-training/positive_images/";
std::string negative_images_path = "/home/sajjadtest/Opencv_Train_Cascade/opencv-haar-classifier-training/negative_images/";

void onMouse(int event, int x, int y, int, void* image)
{
  if (event == CV_EVENT_LBUTTONDOWN)
  {
    std::cout << "-----" << x << "----" << y;
    offset_x = x;
    offset_y = y;
  }
  else if (event == CV_EVENT_MBUTTONDOWN)
  {
    std::cout << "ddddown";
  }
}

class Opencv_Train_Cascade
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;

public:

  Opencv_Train_Cascade() :
    it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/raspberry_pi/image_raw", 1, &Opencv_Train_Cascade::imageCb, this);
    cv::namedWindow(OPENCV_WINDOW);

  }

  ~Opencv_Train_Cascade()
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
    Mat original_rect = img.clone();
    Mat image = img.clone();

    cv::Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;
    roi.width = width;
    roi.height = height;

    Point pt;
    pt.x = offset_x;
    pt.y = offset_y;
    Rect rect(offset_x, offset_y, width, height);
    cv::Mat crop = original_rect(roi);
    setMouseCallback(OPENCV_WINDOW, onMouse, &image);
    cv::imshow("crop", crop);
    rectangle(image, rect, Scalar(0, 255, 255), 1, 8);
    cv::imshow(OPENCV_WINDOW, image);
    int key = waitKey(3);
    //    std::cout << key << std::endl;
    switch (key)
    {
      case 1048695: //W
      {
        offset_x = offset_x - 10;
        offset_y = offset_y - 10;
        width = width + 10;
        height = height + 10;
      }
        break;
      case 1048691: //S
      {
        offset_x = offset_x + 10;
        offset_y = offset_y + 10;
        width = width - 10;
        height = height - 10;
      }
        break;
      case 1048608: //Space
      {
        Mat temp;
        image_name << positive_images_path << number_image << type;
        std::string filename = image_name.str();
        image_name.str("");
        resize(crop, temp, cv::Size(width, height));
        number_image++;
        imwrite(filename, temp);
        std::cout << filename;
      }
        break;
      case 1048676: //D
      {
        width = width + 10;
      }
        break;
      case 1048673: //A
      {
        width = width - 10;
      }
        break;
    }
  }

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "opencv_train_cascade");
  Opencv_Train_Cascade ic;
  ros::spin();
  return 0;
}
