# The original .py file was lost, and it was recovered from the .pyc file by Uncompyle6 #ficaadica

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
from geometry_msgs.msg import PoseArray # sugested ROS Message for future ROS Topic, so custom message isn't necessary

class visionFunctions: # vision algorithms to find objects positions

    def __init__(self, topic, calibration): # right now, it does nothing with calibration; but in the future it may obtain the colors from other-tag colors (mentioned below somewhere), and a bunch of useful info like field limits
        # this color information may be come from calibration in the future:
        self.colors = {'red1': range(11),'orange': range(11, 21),
           'yellow': range(21, 46),
           'green': range(46, 81),
           'cyano': range(81, 105),
           'blue': range(105, 128),
           'purple': range(128, 146),
           'pink': range(146, 170),
           'red2': range(170, 180)
           }
        self.bridge = CvBridge() # used to convert ROS Message image to OpenCV image
        self.sub_img = rospy.Subscriber(topic, Image, self.callback_img) # if u r a ROS noob, you should know that the callback function it's called every time a publisher publishes data

    def callback_img(self, msg_in):
        try:
            img = self.bridge.imgmsg_to_cv2(msg_in, 'bgr8') # to work with ROS and OpenCV, it's necessary to convert from ROS Topic image to OpenCV image
        except CvBridgeError as e:
            print "CvBridge Error:"
            print e
            quit()

        # find poses: (a very useful comment)
        self.find_poses(img)

    def find_poses(self, img):
        # work with HSV can be easier than RGB (or BGR):
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        # create masks for main colors:
        blueMask = self.create_mask(hsv, ['blue'])
        yellowMask = self.create_mask(hsv, ['yellow'])
        orangeMask = self.create_mask(hsv, ['orange'])
        # then for the other-tags colors
        otherColorsMask = self.create_other_colors_mask(img, blueMask, yellowMask, orangeMask)
        # reduce noise from the masks: (for goodness, next time use an array please, you dummy!)
        blueMask = self.noise_reduction(blueMask)
        yellowMask = self.noise_reduction(yellowMask)
        orangeMask = self.noise_reduction(orangeMask)
        otherColorsMask = self.noise_reduction(otherColorsMask)
        # find positions:
        print 'Yellow robots positions:'
        self.find_robots_poses(yellowMask, otherColorsMask)
        print 'Blue robots positions:'
        self.find_robots_poses(blueMask, otherColorsMask)
        print 'Ball position:'
        self.find_ball_pose(orangeMask)
        print '_'

    def create_mask(self, img, color_list):
        # a mask is a binary image that has colors between the threshold below as "true" (or white)
        # e.g. if the color choosen is orange, the binary image will have just the ball (that is orange on VSSS) as white in black background
        color = color_list[0]
        lower_threshold = np.array([self.colors[color][0], 50, 50])
        upper_threshold = np.array([self.colors[color][-1], 255, 255])
        mask = cv2.inRange(img, lower_threshold, upper_threshold)
        return mask

    def create_other_colors_mask(self, img, b, y, o):
        # the threshold below takes grey tones, it aims to take the fild and it's marks
        # this info may also be passed though calibration in the future
        lower_threshold = (0, 0, 0)
        upper_threshold = (180, 50, 50)
        mask = cv2.inRange(img, lower_threshold, upper_threshold)
        # actually we need remove the fild and keep the objects:
        mask = cv2.bitwise_not(mask) # this line inverts the mask, so the fild turn black and the objects white
        # so at this point we got a mask with ALL objects, but let's keep just the unknow ones, removing yellow, blue and orange colors:
        mask = mask - (b + y + o) # YEP! Binary images can be algebraically manipulated
        return mask

    def noise_reduction(self, img):
        element = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
        img = cv2.erode(img, element, iterations=2)
        img = cv2.dilate(img, element, iterations=2)
        img = cv2.erode(img, element)
        return img

    # The VSSS rules say that a robot tag has two colors: one of them, yellow or blue (here called main-colors), and the other one, another color.
    # The following definition matches the main-colors tags with the other-colors tags and gets the robots positions
    def find_robots_poses(self, mainColorMask, otherColorsMask):
        # mc = main-colors
        # oc = other-colors
        # first, get the contours:
        _, contours_mc, hierarchy = cv2.findContours(mainColorMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        _, contours_oc, hierarchy = cv2.findContours(otherColorsMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # now for the three bigger contours:
        i = 0
        while i < 3 and i < len(contours_mc):
            (x, y), radius = cv2.minEnclosingCircle(contours_mc[i]) # finds the smaller circle that encloses the current contour, and gets it's center, and then the center is used as a position
            center = (int(x), int(y))
            radius = int(radius)
            # we need to do some comparations in the next loop, so the next two lines is just to get a reference before begin:
            (X, Y), radius = cv2.minEnclosingCircle(contours_oc[0]) # don't get lost, X and Y are different than x and y
            currentDist = sqrt((x - X) ** 2) + (y - Y) ** 2
            # the next loop finds the smaller distance between the current main-color tag and an other-color tag
            j = 1
            while j < len(contours_oc):
                (X2, Y2), radius = cv2.minEnclosingCircle(contours_oc[j])
                dist = sqrt((x - X2) ** 2) + (y - Y2) ** 2
                if dist < currentDist:
                    currentDist = dist
                    X = X2
                    Y = Y2
                j = j + 1
            centerRobot = (int((X + x) / 2), int((Y + y) / 2)) # the robot's position is the middle point between the two tags
            i = i + 1
            print centerRobot # for now, let's just print it, but in the future this pose may be published on a ROS Topic (I hope)

    # to find the ball pose it's similar, but simpler:
    def find_ball_pose(self, orangeMask):
        _, contours, hierarchy = cv2.findContours(orangeMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        (x, y), radius = cv2.minEnclosingCircle(contours[0])
        center = (int(x), int(y))
        print center # hey you reading this, please publish this position on a ROS Topic and Autobotz will love you forever (or not)

if __name__ == '__main__':
    pass
