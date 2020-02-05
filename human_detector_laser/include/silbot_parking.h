
#ifndef BKK_SILBOT_PARKING
#define BKK_SILBOT_PARKING

//ROS Message
#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Int32.h>
#include <sensor_msgs/LaserScan.h>
#include <visualization_msgs/Marker.h>


//ROS Message
#include "tool_class.h"

class silbot_parking
{
public :
	silbot_parking();
	virtual ~silbot_parking();

	//tool_class
	eun_u::tool_class mat_cal;

	//Set and Get Point Cloud & Ros
	pthread_mutex_t  m_point_mutex = PTHREAD_MUTEX_INITIALIZER;
	ros::NodeHandle nodeHandle; // Topic, service 받기위해 있어야
	ros::Subscriber m_sub_laser_data; //레이저 데이터 구독
	ros::Publisher m_pub_laser_data;  // 레이저 데이터 발행
	ros::Publisher m_pub_result;

	ros::Publisher detect;

	vector<eun_u::Point3D> m_laser_data;

	void laser_data_SubscribeCallBack(const sensor_msgs::LaserScan::ConstPtr& input); // sensor msgs안에 있는 laserScan의 파일에서 가져온다.
public :
	geometry_msgs::Point get_parking_area(sensor_msgs::LaserScan input);


	//Rviz Object tracking Displays
	void axis_display();
	void laser_point_display();
	void object(vector <eun_u::Point3D> start_point, vector <eun_u::Point3D> end_point);
  void human_display(vector <eun_u::Point3D> human_end, vector <eun_u::Point3D> human_start, vector <eun_u::Point3D> start_point, vector <eun_u::Point3D> end_point, vector <eun_u::Point3D> &position);
  void object_detector(vector <eun_u::Point3D> start_point, vector <eun_u::Point3D> end_point, vector <eun_u::Point3D> &position);
	bool positionC(vector <eun_u::Point3D> curpos, vector <eun_u::Point3D> befopos);

private:
	bool m_human_flag;
	double old_secs;
	vector<eun_u::Point3D> m_before_position;
	std_msgs::Int32 a;


};

#endif
