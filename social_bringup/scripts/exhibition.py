#!/usr/bin/python

import rospy
from std_msgs.msg import String, Int8
from social_msgs.srv import SocialMotion
import os
import yaml
import rospkg
import io
import thread
import signal
import time

class Exhibition():

    def __init__(self):
        self.init()

    def load_yaml(self):
        load_dic = {}
        load_file = rospkg.RosPack().get_path('social_bringup')+'/res/exhibition.yaml'
        print 'load data '+load_file
        with io.open(load_file, 'r', encoding='utf8') as stream:
            load_dic = yaml.load(stream)
            # print str(load_dic)
        # print '============================'
        return load_dic

    def on_trigger(self, msg):
        self.__load_dic = self.load_yaml()
        trigger = msg.data
        if trigger in self.__load_dic.keys():
            motion = self.__load_dic[trigger]
            text = '.'
            if motion in self.__load_dic.keys():
                text = self.__load_dic[motion]
            self.__motion_service(motion, text, True)

    def init(self):
        print 'social exhibition v 1.0.0'
        rospy.init_node('social_exhibition', anonymous=True)
        rospy.Subscriber('/social_tele_keyboard/trigger', String, self.on_trigger)
        self.__motion_service = rospy.ServiceProxy('/social_motion_player/play_motion', SocialMotion)
        rospy.spin()

def exit(signum, frame):
    pass

def main():
    signal.signal(signal.SIGINT, exit)
    try:
        Exhibition()
    except rospy.ROSInterruptException:
        pass

if __name__ == '__main__':
    signal.signal(signal.SIGINT, exit)
    try:
        Exhibition()
    except rospy.ROSInterruptException:
        pass
