# -*- coding: utf-8 -*-

import yaml

class autoCalibration():
    def __init__(self, topic):
        imageTopic = self.loadConfig(topic)
        print(imageTopic['topic'][0])

    def loadConfig(self, config):
        try:
            with open(config, 'r') as f:
                return yaml.load(f)
        except:
            print "Error: Invalid configuration file!"
            quit()
