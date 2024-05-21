#ifndef MOBILE_ROBOT_HARDWARE_INTERFACE_H
#define MOBILE_ROBOT_HARDWARE_INTERFACE_H

#include <ros/ros.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <serial/serial.h>
#include <thread>

class MobileRobotHardwareInterface : public hardware_interface::RobotHW
{
public:
    MobileRobotHardwareInterface(ros::NodeHandle& nh);
    ~MobileRobotHardwareInterface();

    void update();
    ros::Time get_time() const;
    ros::Duration get_period() const;

private:
    void read();
    void write();
    void controlLoop();

    ros::NodeHandle nh_;
    serial::Serial ser_;
    std::thread control_loop_;
    bool running_;
    double loop_hz_;

    // Joint state and command interfaces
    hardware_interface::JointStateInterface jnt_state_interface_;
    hardware_interface::VelocityJointInterface jnt_vel_interface_;

    // Joint states and commands
    double pos_left_, vel_left_, eff_left_;
    double pos_right_, vel_right_, eff_right_;  // Add these lines
    double cmd_left_, cmd_right_;
};

#endif // MOBILE_ROBOT_HARDWARE_INTERFACE_H

