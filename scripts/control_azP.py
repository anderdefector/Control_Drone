#!/usr/bin/env python
import rospy
import numpy as np
from std_msgs.msg  import Int32
from std_msgs.msg import Float32

y_d = 320
Kp = 0.004
Kd = 0.0008

global y0
e0 = 0.0

def callback(data):
	global e0
	y = data.data
	e = y_d -y
	de = e - e0
	u = Kp*e + Kd*de
	if abs(e) < 10:
		u=0.0
	if u > 0.1:
		u = 0.1
	elif u < -0.1:
		u = -0.1
	else:
		u = u
	e0 = e
	
	pub.publish(u)
	print 'control az'
	print y,u,e

def main():
	global pub
	pub = rospy.Publisher('controlAZ',Float32,queue_size=10)
	rospy.init_node('control_AZ',anonymous=False)
	rospy.Subscriber('/MX',Int32,callback)
	rospy.spin()

if __name__ == '__main__':
	main()
