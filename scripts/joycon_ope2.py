#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import numpy as np
import time
import rospy

from sensor_msgs.msg import Joy
from nanocraw_ctrl.msg import CrawlerCmd

#command key
joy_left_y  = 1
joy_right_y = 4
joy_right_x = 3
btn_manual   = 1

class joy_to_cmd():
    cmd_msg = CrawlerCmd()
    drive_mode = 0 #0:差動入力, 1:並進・旋回入力 
    chg_cmd_count = 0
 
    def __init__(self):
        self.sub = rospy.Subscriber("joy", Joy, self.callback_joy, queue_size=2) #Joycon 
        self.pub = rospy.Publisher('cugo_cmd', CrawlerCmd, queue_size=1)

    def callback_joy(self, joy_msg):
        if joy_msg.button[btn_manual]==1:
            #manual mode
            self.cmd_msg.left  = int(joy_msg.axes[joy_left_y]  * 800) #convert -1~1 -> -511~511
            self.cmd_msg.right = int(joy_msg.axes[joy_right_y] * 800)
            self.publish()
            self.status_print()
        else:
            if joy_msg.axes[joy_left_y] == 0: #信地旋回
                self.cmd_msg.left  =  int(joy_msg.axes[joy_right_x] * 800)
                self.cmd_msg.right = -int(joy_msg.axes[joy_right_x] * 800)
            else:
                if joy_msg.axes[joy_right_x] >0: #turn right
                    self.cmd_msg.left  = int(joy_msg.axes[joy_left_y] * 800)
                    self.cmd_msg.right = int(joy_msg.axes[joy_left_y] * 800) - int(joy_msg.axes[joy_right_x] * 800)
                else:
                    self.cmd_msg.left  = int(joy_msg.axes[joy_left_y] * 800) + int(joy_msg.axes[joy_right_x] * 800)
                    self.cmd_msg.right = int(joy_msg.axes[joy_left_y] * 800) 

    def publish(self):
        self.pub.publish(self.cmd_msg)
    
    def status_print(self):
        print("[control status]------------------------")
        print("right cmd : %d" %self.cmd_msg.left)
        print("left cmd  : %d" %self.cmd_msg.right)

if __name__ == '__main__':
    rospy.init_node('joycon_ope')
    time.sleep(3.0)
    node = joy_to_cmd()
    while not rospy.is_shutdown():
        rospy.spin()