import yaml

def loadTopic(fileName):
        try:
            with open(fileName, 'r') as f:
                imageTopic = yaml.load(f)
        except:
            print "Can't open topic file"
            quit()
        try:
            return imageTopic['topic'][0]
        except:
            print "Error: Invalid data from file"
            quit()

def loadCalibration(fileName):
        try:
            with open(fileName, 'r') as f:
                calibrationData = yaml.load(f)
        except:
            print "Can't open calibration file"
            quit()
        try:
            # since there's no complete vision algorythm, it won't return data
            print "Calibration file opened sucessfully" # just print this for now...
        except:
            print "Error: Invalid data from file"
            quit()
