#!/usr/bin/env python

import sys
import rospy
import cv2
import numpy as np
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class SubImg():
	def __init__(self):
		#Set up the CV Bridge
		self.bridge = CvBridge()

		# Set up the subscriber
		self.sub_img = rospy.Subscriber('/camera/camera/link/camera/image', Image, self.callback_img)
		self.pub_img = rospy.Publisher('/processed_img', Image, queue_size=10)

	def shutdown(self):
		# Unregister anything that needs it here
		self.sub_img.unregister()

	def callback_img(self, msg_in):
		#Convert ROS image to CV image
		try:
			cv_image = self.bridge.imgmsg_to_cv2( msg_in, "bgr8" )
		except CvBridgeError as e:
			print(e)

		# ===================
		# Do processing here!
		# ===================

		#Convert the current frame to HSV
    		hsv = cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

	    	#Define the threshold for finding a blue object with hsv
	    	lower_blue = np.array([90,25,25])
	    	upper_blue = np.array([135,255,255])

	    	#Create a binary image, where anything blue appears white and everything else is black
	    	mask = cv2.inRange(hsv, lower_blue, upper_blue)

	    	#Get rid of background noise using erosion and fill in the holes using dilation and erode the final image on last time
	    	element = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
	    	mask = cv2.erode(mask,element, iterations=2)
	    	mask = cv2.dilate(mask,element,iterations=2)
	    	mask = cv2.erode(mask,element)

	    	#Create Contours for all blue objects
	    	_, contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	    	maximumArea = 0
	    	bestContour = None
	    	for contour in contours:
			currentArea = cv2.contourArea(contour)
			if currentArea > maximumArea:
		    		bestContour = contour
		    		maximumArea = currentArea
	     	#Create a bounding box around the biggest blue object
	    	if bestContour is not None:
			x,y,w,h = cv2.boundingRect(bestContour)
			cv2.rectangle(cv_image, (x,y),(x+w,y+h), (0,0,255), 1)

		# ===================
		# Show windows!
		# ===================
		if cv2.EVENT_LBUTTONDOWN:
			print("%d, %d\n" % (x,y))
		cv2.imshow('raw',cv_image) #raw
		cv2.imshow('mask', mask) #mask
		cv2.waitKey(1)

		#Convert CV image to ROS image and publish
		try:
			self.pub_img.publish( self.bridge.cv2_to_imgmsg( cv_image, "bgr8" ) )
		except CvBridgeError as e:
			print(e)
