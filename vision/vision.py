# uncompyle6 version 2.16.0
# Python bytecode 2.7 (62211)
# Decompiled from: Python 2.7.12 (default, Dec  4 2017, 14:50:18) 
# [GCC 5.4.0 20160609]
# Embedded file name: /home/victor/Documents/AutoGoal-UFMG/vision/vision.py
# Compiled at: 2018-02-26 15:03:42
import sys, rospy, cv2, os, numpy as np
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from math import sqrt
from geometry_msgs.msg import PoseArray

class visionFunctions:

    def __init__(self, topic, calibration):
        self.colors = {'vermelho1': range(11),'laranja': range(11, 21),
           'amarelo': range(21, 46),
           'verde': range(46, 81),
           'ciano': range(81, 105),
           'azul': range(105, 128),
           'roxo': range(128, 146),
           'rosa': range(146, 170),
           'vermelho2': range(170, 180),
           'cinza': range(180)
           }
        self.bridge = CvBridge()
        self.sub_img = rospy.Subscriber(topic, Image, self.callback_img)

    def callback_img(self, msg_in):
        try:
            img = self.bridge.imgmsg_to_cv2(msg_in, 'bgr8')
        except CvBridgeError as e:
            print e
            quit()

        self.find_poses(img)

    def find_poses(self, im):
        hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
        blueMask = self.create_mask(hsv, ['azul'])
        yellowMask = self.create_mask(hsv, ['amarelo'])
        orangeMask = self.create_mask(hsv, ['laranja'])
        otherColorsMask = self.create_other_colors_mask(im, blueMask, yellowMask, orangeMask)
        blueMask = self.noise_reduction(blueMask)
        yellowMask = self.noise_reduction(yellowMask)
        orangeMask = self.noise_reduction(orangeMask)
        otherColorsMask = self.noise_reduction(otherColorsMask)
        print 'Yellow robots positions:'
        self.find_robots_poses(yellowMask, otherColorsMask)
        print 'Blue robots positions:'
        self.find_robots_poses(blueMask, otherColorsMask)
        print 'Ball position:'
        self.find_ball_pose(orangeMask)
        print '_'

    def create_mask(self, im, color_list):
        color = color_list[0]
        lower_threshold = np.array([self.colors[color][0], 50, 50])
        upper_threshold = np.array([self.colors[color][-1], 255, 255])
        mask = cv2.inRange(im, lower_threshold, upper_threshold)
        return mask

    def create_other_colors_mask(self, im, b, y, o):
        lower_threshold = (0, 0, 0)
        upper_threshold = (180, 50, 50)
        mask = cv2.inRange(im, lower_threshold, upper_threshold)
        mask = cv2.bitwise_not(mask)
        mask = mask - (b + y + o)
        return mask

    def noise_reduction(self, im):
        element = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
        im = cv2.erode(im, element, iterations=2)
        im = cv2.dilate(im, element, iterations=2)
        im = cv2.erode(im, element)
        return im

    def find_robots_poses(self, mainColorMask, otherColorsMask):
        _, contours_mc, hierarchy = cv2.findContours(mainColorMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        _, contours_oc, hierarchy = cv2.findContours(otherColorsMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        i = 0
        while i < 3 and i < len(contours_mc):
            (x, y), radius = cv2.minEnclosingCircle(contours_mc[i])
            center = (int(x), int(y))
            radius = int(radius)
            (X, Y), radius = cv2.minEnclosingCircle(contours_oc[0])
            currentDist = sqrt((x - X) ** 2) + (y - Y) ** 2
            j = 1
            while j < len(contours_oc):
                (X2, Y2), radius = cv2.minEnclosingCircle(contours_oc[j])
                dist = sqrt((x - X2) ** 2) + (y - Y2) ** 2
                if dist < currentDist:
                    currentDist = dist
                    X = X2
                    Y = Y2
                j = j + 1

            centerSecond = (
             int(X), int(Y))
            centerRobot = (int((X + x) / 2), int((Y + y) / 2))
            i = i + 1
            print centerRobot

    def find_ball_pose(self, orangeMask):
        _, contours, hierarchy = cv2.findContours(orangeMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        (x, y), radius = cv2.minEnclosingCircle(contours[0])
        center = (int(x), int(y))
        print center


if __name__ == '__main__':
    pass