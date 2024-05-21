#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import Twist
import serial

def cmd_vel_callback(data):
    linear_x = data.linear.x
    angular_z = data.angular.z
    command = f'{linear_x},{angular_z}\n'
    ser.write(command.encode())

def listener():
    rospy.init_node('motor_control', anonymous=True)

    # Read parameters from launch file
    port = rospy.get_param('~port', '/dev/ttyUSB0')
    baud = rospy.get_param('~baud', 57600)

    # Initialize serial communication
    global ser
    ser = serial.Serial(port, baud)

    rospy.Subscriber('cmd_vel', Twist, cmd_vel_callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
