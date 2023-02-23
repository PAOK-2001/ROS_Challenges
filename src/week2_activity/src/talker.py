#!/usr/bin/env python
import rospy 
from std_msgs.msg import String

if __name__ == '__main__':
    pub = rospy.Publisher("chatter", String, queue_size=10)
    rospy.init_node("talker")
    rate = rospy.Rate(10)
    
    while not rospy.is_shutdown():
        #first param attempt
        #hello_str = rospy.get_param("/Message", "No Parameter Found")  + " " + str(rospy.get_time())
        #global param
        hello_str = "Global param: " + rospy.get_param("/Message", "No Parameter Found")  + " " + str(rospy.get_time())
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        #local param
        hello_str = "Local param: " + rospy.get_param("Message", "No Parameter Found")  + " " + str(rospy.get_time())
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        
        #private param
        hello_str = "Private param: " + rospy.get_param("~Message", "No Parameter Found")  + " " + str(rospy.get_time())
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        
        #hello_str = "hello world %s" %rospy.get_time()
        
        rate.sleep()