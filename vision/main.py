#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, rospy
from auto import autoCalibration
from vision import visionFunctions
from loadYaml import *

# --------------------- #

def main():

	# Initialize ROS node
	rospy.init_node('img_sub_py', anonymous=True)
	rospy.loginfo("Running...")

	# check arguments
	if len(sys.argv) <= 1: # No arguments? Then quit!
		print "Error: Invalid number of arguments!"
		quit()
	elif len(sys.argv) <= 2: # Just topic file? Let's calibrate!
		autoCal = autoCalibration(loadTopic(sys.argv[1]))
	else: # Topic file and calibration file? Let's run it
		playVision = visionFunctions(loadTopic(sys.argv[1]),loadCalibration(sys.argv[2]))

	# Loop here until quit
	try:
		rospy.spin()
	except rospy.ROSInterruptException:
		# Shutdown
		rospy.loginfo("Shutting down...")
		sp.shutdown()
		rospy.loginfo("Done!")

# --------------------- #

if __name__ == '__main__':
    main()
