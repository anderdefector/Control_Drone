#!/usr/bin/env python

#Motion control in x axis of parrot bebop using orb slam feedback
#Working!! 15/02/2018
import rospy
import numpy as np
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

x_d = 0
Kp = 0.6
Kd = 0.1
global x0
e0 = 0.0

vel_msg = Twist()

def callback(data):
	global e0
	xs = np.fromstring(data.data, dtype=float, sep=',')
	x = -xs[11]
	e = x_d - x
	de = e - e0
	u = Kp*e + Kd*de
	if abs(e) < 0.05:
		u= 0.0
	if u > 0.2:
		u = 0.2
	elif u < -0.2:
		u = -0.2
	else:
		u = u
	e0 = e
	#vel_msg.linear.x = u
	pub.publish(u)
	#pub.publish(str(u))
	print 'control x'
	print x,u
	
def main():
	global pub
	pubx = rospy.Publisher('posX',Float32,queue_size=10)
	pub = rospy.Publisher('controlX',Float32,queue_size=10)
	#pub = rospy.Publisher('/bebop/cmd_vel',Twist,queue_size=10)
	#pub = rospy.Publisher('controlOrbSX',String,queue_size=10)
	rospy.init_node('control_orbslam_x',anonymous=False)
	rospy.Subscriber('slam_bebop_pose',String,callback) 
	rospy.spin()

if __name__ == '__main__':
	main()
