#! /usr/bin/env python
import rospy
from std_msgs.msg import Float32


def signalCallback(signal):
    rospy.loginfo(signal)


if __name__ == '__main__':
    rospy.init_node("signal_processor")
    sub = rospy.Subscriber("/signal", Float32 ,signalCallback)
    rospy.Rate(20)
    rospy.spin()
