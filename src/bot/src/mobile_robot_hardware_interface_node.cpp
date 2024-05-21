#include <ros/ros.h>
#include <controller_manager/controller_manager.h>
#include "mobile_robot_hardware_interface.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "robot_hardware_interface_node");
    ros::NodeHandle nh;

    // Create hardware interface instance
    MobileRobotHardwareInterface robot(nh);

    // Create controller manager instance
    controller_manager::ControllerManager cm(&robot, nh);

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Rate rate(10); // 10 Hz
    while (ros::ok())
    {
        // Update hardware interface and controller manager
        robot.update();
        cm.update(robot.get_time(), robot.get_period());

        // Sleep to maintain loop rate
        rate.sleep();
    }

    return 0;
}
