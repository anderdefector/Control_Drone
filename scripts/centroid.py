#!/usr/bin/env python
#--------------color centroid detection for tracking--------
#--------------17/03/2018-----------------------------
#--------------library
import cv2
import numpy as np

def load():
    files = open('Segm.txt','r')
    msg = files.readlines()
    for data in msg:
        tbpos = data.rsplit(',')
    return tbpos
    print tbpos

global pos
#pos = load()
#pos = np.array([])

def segmentacao(image):
	global pos
	#-------Segmentation--------------------
	#R = int(pos[0]); G = int(pos[1]); B = int(pos[2]); F = int(pos[6]);
	#r = int(pos[3]); g = int(pos[4]); b = int(pos[5]);
	lower = np.array([102,0,139])
	upper = np.array([161,255,255])
	seg = cv2.inRange(cv2.cvtColor(image,cv2.COLOR_BGR2HSV),lower,upper)
	seg = cv2.medianBlur(seg,2*F+1)
	return seg
	
def centroid(seg,img):
	#-------Contours------------------------
	image = img.copy()
	contours=[]
	cX=[]
	cY=[]
	cont=cv2.findContours(seg,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)

	for c in cont[1]:
		areas =cv2.contourArea(c)
		M=cv2.moments(c)
		if M["m00"]>5000:
			contours.append(c)
			cX.append(int(M["m10"] / M["m00"]))
			cY.append(int(M["m01"] / M["m00"]))
			#cv2.circle(image, (int(cX[0]), int(cY[0])), 7, (0,255,0), -1)
	cv2.drawContours(image,contours,-1,(0,255,0),3)
	return cX,cY,image

