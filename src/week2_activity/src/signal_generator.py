#!/usr/bin/env python
import rospy
import numpy as np
from week2_activity.msg import signal_msg  #Import the message to be used

if __name__=='__main__':

	## Declare the new message to be used
    signal_pub=rospy.Publisher("signal",signal_msg, queue_size=10)    

    rospy.init_node("signal_generator")
    rate = rospy.Rate(10)
    init_time = rospy.get_time()
    msg = signal_msg()

    while not rospy.is_shutdown():
        time = rospy.get_time()-init_time
        signal = np.sin(time)

	    ## Fill the message with the required information	
        msg.time_x = time
        msg.signal_y = signal

	    ## Publish the message
        signal_pub.publish(msg)

        rospy.loginfo("The signal value is: %f at a time %f", signal, time)
        rate.sleep()