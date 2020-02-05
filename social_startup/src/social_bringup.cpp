#include "social_bringup.h"

/*
 * driving_test.cpp
 * update date : 12. 09
 *      Author : dj_jin
 * update 내용  : CES용 SOCIAL BRINGUP CPP
 */

social_bringup::social_bringup()
{
    init_node();

}

social_bringup::~social_bringup()
{

}

/*****************************************************************************
 ** Init Function
 *****************************************************************************/
void social_bringup::init_node()
{
    m_pub_tts = m_nodeHandle.advertise<robocare_msgs::TTSSetProperties>("/robocare_tts/set_properties",1);

    _tts_set.pitch = 100;
    _tts_set.speed = 90;
    _tts_set.volume = 100;
    _tts_set.speaker_id = 3;

    ROS_WARN("CONNECT SUCCEED TTS");

    m_motion_client = m_nodeHandle.serviceClient<social_msgs::SocialMotion>("/social_motion_player/play_motion");
    m_sub_detect = m_nodeHandle.subscribe("/human_detect", 1, &social_bringup::detect_SubscribeCallback, this);

//    int i = rand() % 2 + 1;

    _motion[0].request.text = "Hello, My name is Ah-RO.";
    _motion[0].request.file_name = "right_ment_1";

//    if(i == 1) {
//      printf("1 receive !!\n");
//      _motion[0].request.file_name = "right_ment_1";
//    }
//    else _motion[0].request.file_name = "left_ment_1";

    _motion[1].request.text = "I am a robot of robo care. Nice to meet you!";
    _motion[1].request.file_name = "right_ment_2";

    _motion[2].request.text = "Robo care is a company that provides robot services to make humans more happier.";
    _motion[2].request.file_name = "ment_3";

    _motion[3].request.text = "Please allow me to introduce my freind BOMY.";
    _motion[3].request.file_name = "left_ment_4";

    _motion[4].request.text = "BOMY is a personal assistant and cognitive trainer for people in risk of onset of dementia. ";
    _motion[4].request.file_name = "ment_3";

    _motion[5].request.text = "There is a BOMY over there. Please come in and meet BOMY. Thank you.";
    _motion[5].request.file_name = "left_ment_6";

}

/*****************************************************************************
 ** Key Function
 *****************************************************************************/

void social_bringup::detect_SubscribeCallback(const std_msgs::Int32 &input)
{

  m_detect = input.data;
  human_flag = (m_detect == 1) ? true : false ;

  printf("Detect : %d \n" ,m_detect);
  if(human_flag == true)  {
    m_pub_tts.publish(_tts_set);
    ros::Duration(1.0).sleep();
    play_motion();
  }
}

void social_bringup::play_motion()
{
  printf("human!!\t");

  ros::Duration(0.5).sleep();
  m_motion_client.call(_motion[0]);
  ros::Duration(1.0).sleep();

  m_motion_client.call(_motion[1]);
  ros::Duration(1.0).sleep();

  m_motion_client.call(_motion[2]);
  ros::Duration(1.0).sleep();

  m_motion_client.call(_motion[3]);
  ros::Duration(1.0).sleep();

  m_motion_client.call(_motion[4]);
  ros::Duration(1.0).sleep();

  m_motion_client.call(_motion[5]);
  ros::Duration(1.0).sleep();
}

int main(int argc, char** argv) {

    cout << "SOCIAL BRINGUP" <<endl;
    ros::init(argc, argv, "SOCIAL_BRINGUP_BODE");

    social_bringup SB;

    ros::MultiThreadedSpinner spinner(1); // Use 4 threads
    spinner.spin(); // spin() will not return until the node has been shutdown

    return 0;
}
