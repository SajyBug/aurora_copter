#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>

void poseCallback(const geometry_msgs::PoseStamped::ConstPtr &pose)
{
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  transform.setOrigin(tf::Vector3(pose->pose.position.x, pose->pose.position.y, pose->pose.position.z));
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "copter_frame"));
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "copter_frame");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("/mavros/local_position/pose", 10, poseCallback);
  ros::spin();
  return 0;
};
