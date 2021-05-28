#!/usr/bin/env python

import serial
import time
import rospy
import time
import math
import numpy as np
import sys, select, os
from std_msgs.msg import Int32
from std_msgs.msg import String
from std_msgs.msg import Float32
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

if os.name == 'nt':
  import msvcrt
else:
  import tty, termios

BURGER_MAX_LIN_VEL = 0.22 #0.22
BURGER_MAX_ANG_VEL = 2.84  #2.84

WAFFLE_MAX_LIN_VEL = 0.26
WAFFLE_MAX_ANG_VEL = 1.82

LIN_VEL_STEP_SIZE = 0.01
ANG_VEL_STEP_SIZE = 0.2#0.2

message = ""

msg = """
Teleoperation of Turtlebot using tactile glove

CTRL-C to quit
"""

e = """
Communications Failed
"""

readSer = serial.Serial('/dev/ttyUSB0',9600)		# Serial Communication Port for Controlling turtlebot
writeSer = serial.Serial('/dev/ttyUSB1',9600)		# UART Communication Port for LaserScan data for vibromotors

time.sleep(2)						# wait for setup finished
def getKey():						# method to get control flags from glove
	message = readSer.readline().decode().rstrip()# Read Serial port
	print("Recieve:", message)			# Display recieved message
	return message					# Return recieved message

def getMin(arr, start, end):				# function for getting the minimum value from given range
	minval = 500.0		
	for i in range(start, end):
		if(arr.ranges[i] < minval):
			minval = arr.ranges[i]
	return minval
		
    

def vels(target_linear_vel, target_angular_vel):
    return "currently:\tlinear vel %s\t angular vel %s " % (target_linear_vel,target_angular_vel)

def makeSimpleProfile(output, input, slop):
    if input > output:
        output = min( input, output + slop )
    elif input < output:
        output = max( input, output - slop )
    else:
        output = input

    return output

def constrain(input, low, high):
    if input < low:
      input = low
    elif input > high:
      input = high
    else:
      input = input

    return input

def checkLinearLimitVelocity(vel):
    if turtlebot3_model == "burger":
      vel = constrain(vel, -BURGER_MAX_LIN_VEL, BURGER_MAX_LIN_VEL)
    elif turtlebot3_model == "waffle" or turtlebot3_model == "waffle_pi":
      vel = constrain(vel, -WAFFLE_MAX_LIN_VEL, WAFFLE_MAX_LIN_VEL)
    else:
      vel = constrain(vel, -BURGER_MAX_LIN_VEL, BURGER_MAX_LIN_VEL)

    return vel

def callback(msg):	# callback function to retrieve laserscan data and send to arduino board
    front = str(min(getMin(msg, 0, 45), getMin(msg, 315, 360))) # data 2
    left = str(getMin(msg, 45, 135))  # data 3
    right = str(getMin(msg, 225, 315)) # data 1
    # Data: "data1|data2|data3#"
    current = right + "|" + front + "|" + left + "#"
    print("send:\t"+current)
    writeSer.write(current.encode())


def checkAngularLimitVelocity(vel):
    if turtlebot3_model == "burger":
      vel = constrain(vel, -BURGER_MAX_ANG_VEL, BURGER_MAX_ANG_VEL)
    elif turtlebot3_model == "waffle" or turtlebot3_model == "waffle_pi":
      vel = constrain(vel, -WAFFLE_MAX_ANG_VEL, WAFFLE_MAX_ANG_VEL)
    else:
      vel = constrain(vel, -BURGER_MAX_ANG_VEL, BURGER_MAX_ANG_VEL)

    return vel


if __name__=="__main__":
    if os.name != 'nt':
        settings = termios.tcgetattr(sys.stdin)

    rospy.init_node('turtlebot3_teleop')
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    sub = rospy.Subscriber('scan', LaserScan, callback)
    turtlebot3_model = rospy.get_param("model", "burger")

    status = 0
    target_linear_vel   = 0.0
    target_angular_vel  = 0.0
    control_linear_vel  = 0.0
    control_angular_vel = 0.0

    try:
        print(msg)
        while(1):
            key = getKey()
            
            if key == 'w' :
                target_linear_vel = checkLinearLimitVelocity(target_linear_vel + LIN_VEL_STEP_SIZE)
                status = status + 1
            elif key == 'x' :
                target_linear_vel = checkLinearLimitVelocity(target_linear_vel - LIN_VEL_STEP_SIZE)
                status = status + 1
            elif key == 'a' :
                target_angular_vel = checkAngularLimitVelocity(target_angular_vel + ANG_VEL_STEP_SIZE)
                status = status + 1
            elif key == 'd' :
                target_angular_vel = checkAngularLimitVelocity(target_angular_vel - ANG_VEL_STEP_SIZE)
                status = status + 1
            elif key == ' ' or key == 's' :
                target_linear_vel   = 0.0
                control_linear_vel  = 0.0
            elif key == 'r' :
                target_angular_vel  = 0.0
                control_angular_vel = 0.0   
            else:
                continue
            print(vels(target_linear_vel, target_angular_vel)) 
	
            if status == 20 :
                print(msg)
                status = 0

            twist = Twist()

            control_linear_vel = makeSimpleProfile(control_linear_vel, target_linear_vel, (LIN_VEL_STEP_SIZE/2.0))
            twist.linear.x = control_linear_vel; twist.linear.y = 0.0; twist.linear.z = 0.0

            control_angular_vel = makeSimpleProfile(control_angular_vel, target_angular_vel, (ANG_VEL_STEP_SIZE/2.0))
            twist.angular.x = 0.0; twist.angular.y = 0.0; twist.angular.z = control_angular_vel
            pub.publish(twist)

    except:
        print(e)

    finally:
        twist = Twist()
        twist.linear.x = 0.0; twist.linear.y = 0.0; twist.linear.z = 0.0
        twist.angular.x = 0.0; twist.angular.y = 0.0; twist.angular.z = 0.0
        pub.publish(twist)

    if os.name != 'nt':
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)

