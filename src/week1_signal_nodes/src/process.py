#! /usr/bin/env python
import rospy
import threading as thrd
from std_msgs.msg import Float32

signal_data = 0
    
def signalCallback(signal):
    global signal_data
    signal_data = signal.data
    rospy.loginfo(signal)

if __name__ == '__main__':
    rospy.init_node("signal_processor")
    pub = rospy.Publisher("/procSignal", Float32, queue_size=20)
    signalSub = rospy.Subscriber("/signal", Float32 ,signalCallback)
    time = rospy.Subscriber("/time", Float32)
    rate = rospy.Rate(20)
    while not rospy.is_shutdown():
        proc = (signal_data+2.224) *1.1518
        pub.publish(proc)
        rate.sleep()
