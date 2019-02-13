#!/usr/bin/env python
#------------------
import rospy
import cv2
import numpy as np
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import centroid as cnt
#import vlidar as vl

br = CvBridge()

def img_callback(data):
	try:
		image = br.imgmsg_to_cv2(data,"bgr8")
		segm = cnt.segmentacao(image)
		cv2.imshow("seg",segm)
		cx,cy,seg = cnt.centroid(segm,image)
		cx = np.asarray(cx)
		cy = np.asarray(cy)
		_cx = 0
		_cy = 0
		if not cx is None and not cy is None:
			for k in range(len(cx)):
				_cx = (_cx+cx[k])/(k+1)
				_cy = (_cy+cy[k])/(k+1)
				cv2.circle(seg, (int(cx[k]), int(cy[k])), 7, (0,255,0), -1)
		cv2.circle(seg, (int(_cx), int(_cy)), 7, (0,255,0), -1)		
		print _cy,_cx
		'''
		if not cx is None and not cy is None:
			for k in range(cx):
				cv2.circle(image, (int(cx[k]), int(cy[k])), 7, (0,255,0), -1)
		'''
		cv2.imshow("frame",seg)
		cv2.waitKey(1)
	except CvBridgeError, e:
		print e
		
def main():
	global pub
	rospy.init_node('MainBebop',anonymous=False)
	#rospy.Subscriber("/usb_cam/image_raw",Image,img_callback)
	rospy.Subscriber("/bebop/image_raw",Image,img_callback)
	#rospy.Publish
	
	rospy.spin()

if __name__ == '__main__':
	main()
