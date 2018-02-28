# -*- coding: utf-8 -*-

import sys, rospy, cv2
import numpy as np
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class autoCalibration():
    def __init__(self, topic):
        self.bridge = CvBridge()
        self.sub_img = rospy.Subscriber(topic, Image, self.callback_img)

    def callback_img(self, msg_in):
        # convert from ROS message to OpenCV image
        try:
            img = self.bridge.imgmsg_to_cv2( msg_in, "bgr8" )
            img, res2 = self.kmeans(img)
            cv2.imshow('res2',res2)
            cv2.imshow('img',img)
            cv2.waitKey(0)
            cv2.destroyAllWindows()
        except CvBridgeError as e:
            print(e)

    def kmeans(self,img):
        # K-means method can be used to get most common color on image for future implementation:
        # The k-means method:
        Z = img.reshape((-1,3))

        # convert to np.float32
        Z = np.float32(Z)

        # define criteria, number of clusters(K) and apply kmeans()
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
        K = 4
        ret,label,center=cv2.kmeans(Z,K,None,criteria,10,cv2.KMEANS_RANDOM_CENTERS)

        # Now convert back into uint8, and make original image
        center = np.uint8(center)
        res = center[label.flatten()]
        res2 = res.reshape((img.shape))

        return img, res2
