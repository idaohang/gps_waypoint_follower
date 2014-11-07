#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "gps_follower");

  ros::NodeHandle node;

 
  ros::Publisher bot_vel =  node.advertise<geometry_msgs::Twist>("husky/cmd_vel", 10);

  tf::TransformListener listener;

  ros::Rate rate(10.0);

  while (node.ok())
  {
    tf::StampedTransform transform;
    try
    {
        listener.waitForTransform("/current_goal", "/current_pose", ros::Time(0), ros::Duration(10.0) );
        listener.lookupTransform("/current_goal", "/current_pose", ros::Time(0), transform);
    }
    catch (tf::TransformException ex) 
    {
        ROS_ERROR("%s",ex.what());
    }
    geometry_msgs::Twist vel_msg;
    vel_msg.angular.z = 4.0 * atan2(transform.getOrigin().y(),
                                    transform.getOrigin().x());
    vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
                                  pow(transform.getOrigin().y(), 2));
    if(vel_msg.linear.x > 0.8)
    {
      vel_msg.linear.x = 0.8;
    }

    bot_vel.publish(vel_msg);

    rate.sleep();
  }
  return 0;
};