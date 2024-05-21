#include "mobile_robot_hardware_interface.h"
#include <std_msgs/String.h>
#include <sstream>

MobileRobotHardwareInterface::MobileRobotHardwareInterface(ros::NodeHandle& nh)
    : nh_(nh), loop_hz_(10), running_(true), pos_left_(0), vel_left_(0), eff_left_(0), cmd_left_(0), cmd_right_(0),
      pos_right_(0), vel_right_(0), eff_right_(0)  // Initialize these variables
{
    // Initialize serial communication
    std::string port;
    int baud;
    nh_.param<std::string>("port", port, "/dev/ttyUSB0");
    nh_.param<int>("baud", baud, 57600);

    try
    {
        ser_.setPort(port);
        ser_.setBaudrate(baud);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser_.setTimeout(to);
        ser_.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
    }

    if (ser_.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
        ROS_ERROR_STREAM("Failed to open serial port");
    }

    // Initialize hardware interface and joint state interface
    hardware_interface::JointStateHandle state_handle_left("left_wheel_joint", &pos_left_, &vel_left_, &eff_left_);
    jnt_state_interface_.registerHandle(state_handle_left);

    hardware_interface::JointStateHandle state_handle_right("right_wheel_joint", &pos_right_, &vel_right_, &eff_right_);
    jnt_state_interface_.registerHandle(state_handle_right);

    hardware_interface::JointHandle vel_handle_left(jnt_state_interface_.getHandle("left_wheel_joint"), &cmd_left_);
    jnt_vel_interface_.registerHandle(vel_handle_left);

    hardware_interface::JointHandle vel_handle_right(jnt_state_interface_.getHandle("right_wheel_joint"), &cmd_right_);
    jnt_vel_interface_.registerHandle(vel_handle_right);

    registerInterface(&jnt_state_interface_);
    registerInterface(&jnt_vel_interface_);

    // Start control loop
    control_loop_ = std::thread(&MobileRobotHardwareInterface::controlLoop, this);
}

MobileRobotHardwareInterface::~MobileRobotHardwareInterface()
{
    running_ = false;
    control_loop_.join();
}

void MobileRobotHardwareInterface::update()
{
    // Update joint state
    read();
}

ros::Time MobileRobotHardwareInterface::get_time() const
{
    return ros::Time::now();
}

ros::Duration MobileRobotHardwareInterface::get_period() const
{
    return ros::Duration(1.0 / loop_hz_);
}

void MobileRobotHardwareInterface::read()
{
    // Read data from the serial port
    if (ser_.available())
    {
        std::string result = ser_.readline(1000, "\n");
        // Parse the result and update joint states
        std::stringstream ss(result);
        ss >> pos_left_ >> vel_left_ >> pos_right_ >> vel_right_;  // Read right joint data
    }
}

void MobileRobotHardwareInterface::controlLoop()
{
    ros::Rate rate(loop_hz_);
    while (running_ && ros::ok())
    {
        // Write commands to the robot
        write();

        rate.sleep();
    }
}

void MobileRobotHardwareInterface::write()
{
    // Write commands to the serial port
    std::stringstream ss;
    ss << cmd_left_ << "," << cmd_right_ << "\n";
    ser_.write(ss.str());
}
