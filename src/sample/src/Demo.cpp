#include <chrono>
#include <cmath>
#include <thread>
#include <boost/shared_ptr.hpp>

#include <ros/ros.h>
#include <ros/package.h>


int main(int argc, char **argv) {


    ros::init(argc, argv, "Sample");

    ros::NodeHandle nh;
    
    
#ifdef TARGET_AUV_4
 
    ROS_INFO("Build for AUV 4.0");
#endif

#ifdef TARGET_ASV_2
 
    ROS_INFO("Build for ASV 2.0");
    
#endif


#ifdef TARGET_AUV_3

    ROS_INFO("Build for AUV 3.0");

#endif

    return 0;
}

