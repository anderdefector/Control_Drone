#!/usr/bin/env python
#--------------color segmentation for tracking----------------
#--------------17/03/2018-------------------------------------
#------Node: segmentacao--------------------------------------
#------Subscribes to: /bebop/image_raw---/usb_cam/image_raw---
#------Publishes: nothing!!-----------------------------------
import rospy
import cv2
import numpy as np
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

global k
k = 0

def nothing(x):
	pass

def save(R,G,B,r,g,b,F):
    files = open('Segm.txt','w')
    files.write(str(R)+','+str(G)+','+str(B)+','+str(r)+','+str(g)+','+str(b)+"," +str(F))
    files.close()
    
def load():
    files = open('Segm.txt','r')
    msg = files.readlines()
    for data in msg:
        tbpos = data.rsplit(',')
    return tbpos
    print tbpos

class Segmentation:
	def __init__(self):
		self.br = CvBridge()
		#self.img = rospy.Subscriber("/bebop/image_raw",Image,self.img_callback)
		self.img = rospy.Subscriber("/usb_cam/image_raw",Image,self.img_callback)
	def img_callback(self, data):
		global k
		try:
			image = self.br.imgmsg_to_cv2(data,"bgr8")
			minimage = cv2.resize(image.copy(),(320, 240), cv2.INTER_LINEAR)
			cv2.imshow("frame",minimage)
			if k == 0:
				cv2.createTrackbar('R','frame',0,255,nothing)
				cv2.createTrackbar('G','frame',0,255,nothing)
				cv2.createTrackbar('B','frame',0,255,nothing)
				cv2.createTrackbar('r','frame',0,255,nothing)
				cv2.createTrackbar('g','frame',0,255,nothing)
				cv2.createTrackbar('b','frame',0,255,nothing)
				cv2.createTrackbar('F','frame',0,10,nothing)
				pos = load()
				cv2.setTrackbarPos("R","frame",int(pos[0]))
				cv2.setTrackbarPos("G","frame",int(pos[1]))
				cv2.setTrackbarPos("B","frame",int(pos[2]))
				cv2.setTrackbarPos("r","frame",int(pos[3]))
				cv2.setTrackbarPos("g","frame",int(pos[4]))
				cv2.setTrackbarPos("b","frame",int(pos[5]))
				cv2.setTrackbarPos("F","frame",int(pos[6]))
				k = 1
			
			R = cv2.getTrackbarPos('R','frame')
			G = cv2.getTrackbarPos('G','frame')
			B = cv2.getTrackbarPos('B','frame')
			r = cv2.getTrackbarPos('r','frame')
			g = cv2.getTrackbarPos('g','frame')
			b = cv2.getTrackbarPos('b','frame')
			F = cv2.getTrackbarPos('F','frame')
	
			lower = np.array([b,g,r])
			upper = np.array([B,G,R])
			seg = cv2.inRange(image,lower,upper)
			seg = cv2.medianBlur(seg,2*F+1)
			
			cv2.imshow("seg",seg)
			if cv2.waitKey(5) & 0xFF == ord('s'):
				save(R,G,B,r,g,b,F)
				print 'Calibration saved!'
		except CvBridgeError, e:
			print e

def main():
	rospy.init_node('segmentacao',anonymous=False)
	seg = Segmentation()
	rospy.spin()

if __name__ == '__main__':
	main()
