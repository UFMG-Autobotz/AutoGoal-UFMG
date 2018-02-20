# -*- coding: utf-8 -*-

import sys
import rospy
import cv2
import numpy as np
import yaml
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class autoCalibration():
    def __init__(self, topic):
        self.bridge = CvBridge()
        imageTopic = self.loadConfig(topic)
        print(imageTopic['topic'][0]) # print topic from yaml
        self.sub_img = rospy.Subscriber(imageTopic['topic'][0], Image, self.callback_img)

    def loadConfig(self, config):
        try:
            with open(config, 'r') as f:
                return yaml.load(f)
        except:
            print "Error: Invalid configuration file!"
            quit()

    def callback_img(self, msg_in):
        try:
            cv_image = self.bridge.imgmsg_to_cv2( msg_in, "bgr8" )
        except CvBridgeError as e:
            print(e)
        cv2.imshow('image',cv_image)
        cv2.waitKey(1)
