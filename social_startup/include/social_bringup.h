#ifndef SOCAIL_BRINGUP_H
#define SOCAIL_BRINGUP_H

#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <social_msgs/SocialMotion.h>
#include <social_msgs/SocialMotionRequest.h>
#include <social_msgs/SocialMotionResponse.h>
#include <robocare_msgs/TTSSetProperties.h>

using namespace std;

class social_bringup
{

private:

    ros::NodeHandle m_nodeHandle;
    ros::ServiceClient m_motion_client;
    ros::Publisher m_pub_tts;
    ros::Subscriber m_sub_detect;

    social_msgs::SocialMotion _motion[6];

    robocare_msgs::TTSSetProperties _tts_set;




public:

    social_bringup();
    virtual ~social_bringup();

    void play_motion();


    // init_node
    void init_node();

    void detect_SubscribeCallback(const std_msgs::Int32& input);

    int m_detect;

    bool human_flag;

};




#endif // SOCAIL_BRINGUP_H
