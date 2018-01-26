#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
from auto import autoCalibration

# --------------------- #

def main():
    # quit if a config file is not sent
    if len(sys.argv) <= 1:
        print "Error: Invalid number of arguments!"
        quit()
    try:
        print(sys.argv[2])
    except:
        autoCal = autoCalibration(sys.argv[1])
        # autoCal.calibrate()


    #if '-a' in sys.argv:
    #    calibration.auto


# --------------------- #

if __name__ == '__main__':
    main()
