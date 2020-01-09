/* Copyright (c) 2018, Yu Zhang, Intelligent Vehicle Research Center(IVRC), 
 * Beijing Institute of Technology.
 * All rights reserved.
 *        Files: simple_node.cpp
 *   Created on: 10, 02, 2018 
 *       Author: Yu Zhang
 *        Email: yu.zhang.bit@gmail.com
 */
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <boost/thread.hpp>

// #include <sstream>
static unsigned int callback1_count = 0;
static unsigned int callback2_count = 0;
static unsigned int callback3_count = 0;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void subscriberCallback1(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO_STREAM("First subscriber callback: "
                            << callback1_count
                            << ", in thread: " << boost::this_thread::get_id());
    callback1_count++;
    ros::Rate loop_rate(5);
    // sleep for 200ms
    loop_rate.sleep();
}

void subscriberCallback2(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO_STREAM("Second subscriber callback: "
            << callback2_count << ", in thread: "
            << boost::this_thread::get_id());
    callback2_count++;
    ros::Rate loop_rate(5);
    // sleep for 200ms
    loop_rate.sleep();
}

void subscriberCallback3(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO_STREAM("Third subscriber callback: "
            << callback3_count << ", in thread: "
            << boost::this_thread::get_id());
    callback3_count++;
    ros::Rate loop_rate(5);
    // sleep for 200ms
    loop_rate.sleep();
}


/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
    /**
     * The ros::init() function needs to see argc and argv so that it can perform
     * any ROS arguments and name remapping that were provided at the command line.
     * For programmatic remappings you can use a different version of init() which takes
     * remappings directly, but for most command-line programs, passing argc and argv is
     * the easiest way to do it.  The third argument to init() is the name of the node.
     *
     * You must call one of the versions of ros::init() before using any other
     * part of the ROS system.
     */
    ros::init(argc, argv, "talker");
    
    /**
     * NodeHandle is the main access point to communications with the ROS system.
     * The first NodeHandle constructed will fully initialize this node, and the last
     * NodeHandle destructed will close down the node.
     */
    ros::NodeHandle n;
    
    /**
     * The advertise() function is how you tell ROS that you want to
     * publish on a given topic name. This invokes a call to the ROS
     * master node, which keeps a registry of who is publishing and who
     * is subscribing. After this advertise() call is made, the master
     * node will notify anyone who is trying to subscribe to this topic name,
     * and they will in turn negotiate a peer-to-peer connection with this
     * node.  advertise() returns a Publisher object which allows you to
     * publish messages on that topic through a call to publish().  Once
     * all copies of the returned Publisher object are destroyed, the topic
     * will be automatically unadvertised.
     *
     * The second parameter to advertise() is the size of the message queue
     * used for publishing messages.  If messages are published more quickly
     * than we can send them, the number here specifies how many messages to
     * buffer up before throwing some away.
     */
    ros::Publisher periodic_pub = n.advertise<std_msgs::String>("/publisher", 1);
    ros::Subscriber sub1 = n.subscribe<std_msgs::String>("/subscriber",
                                                         1,
                                                         subscriberCallback1);
    ros::Subscriber sub2 = n.subscribe<std_msgs::String>("/subscriber",
                                                         1,
                                                         subscriberCallback2);
    ros::Subscriber sub3 = n.subscribe<std_msgs::String>("/subscriber",
                                                         1,
                                                         subscriberCallback3);
    ros::Rate loop_rate(10);
    
    /**
     * A count of how many messages we have sent. This is used to create
     * a unique string for each message.
     */
    int count = 0;
    
    ROS_INFO_STREAM("Main loop in thread: " << boost::this_thread::get_id());
    while (ros::ok())
    {
        /**
         * This is a message object. You stuff it with data, and then publish it.
         */
        std_msgs::String msg;
        
        std::stringstream ss;
        ss << "Periodic Publisher " << count;
        msg.data = ss.str();
        
        ROS_INFO_STREAM("" << msg.data.c_str() <<", in thread: "
            << boost::this_thread::get_id());
        
        /**
         * The publish() function is how you send messages. The parameter
         * is the message object. The type of this object must agree with the type
         * given as a template parameter to the advertise<>() call, as was done
         * in the constructor above.
         */
        periodic_pub.publish(msg);
        
        ros::spinOnce();
        
        loop_rate.sleep();
        ++count;
    }
    return 0;
}
