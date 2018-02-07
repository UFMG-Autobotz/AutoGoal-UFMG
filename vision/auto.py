# -*- coding: utf-8 -*-

import yaml, rospy, cv2
from cv_bridge import CvBridge, CvBridgeError

class autoCalibration():
    def __init__(self, topic):
        imageTopic = self.loadConfig(topic)
        print(imageTopic['topic'][0])
        self.image_sub = rospy.Subscriber(imageTopic['topic'][0], Image, callback_img)


    def loadConfig(self, config):
        try:
            with open(config, 'r') as f:
                return yaml.load(f)
        except:
            print "Error: Invalid configuration file!"
            quit()

    def set_img_source(self):
    		def callback_img(data):
    			try:
    				self.img_orig = self.bridge.imgmsg_to_cv2(data, "bgr8")
    			except CvBridgeError as e:
    				print(e)
    		self.image_sub = rospy.Subscriber(str(self.img_source.currentText()), Image, callback_img)
