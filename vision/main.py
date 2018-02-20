#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, rospy
from auto import autoCalibration

# --------------------- #

def main():
    # Initialize ROS node
	rospy.init_node('img_sub_py', anonymous=True)
    # quit if a config file is not sent
	if len(sys.argv) <= 1:
		print "Error: Invalid number of arguments!"
		quit()
    # check if there's a second argument (calibration file)
	try:
		print(sys.argv[2])
    # if there isn't, it'll start auto calibration
	except:
		autoCal = autoCalibration(sys.argv[1]) # send image topic file
        # autoCal.calibrate()

	rospy.loginfo("Running...")
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
