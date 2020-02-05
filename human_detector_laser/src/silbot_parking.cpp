#include "silbot_parking.h"

silbot_parking::silbot_parking()
{

  if ((int)m_laser_data.size() > 0)
    m_laser_data.clear();
  if ((int)m_before_position.size() > 0)
    m_before_position.clear();
  this->m_sub_laser_data = this->nodeHandle.subscribe<sensor_msgs::LaserScan>("/scan", 1, &silbot_parking::laser_data_SubscribeCallBack, this);
  this->m_pub_laser_data = this->nodeHandle.advertise<visualization_msgs::Marker>("/silbot_parking/display_data", 10);
  this->detect = this->nodeHandle.advertise<std_msgs::Int32>("/human_detect", 1);

  m_human_flag = false;
  old_secs = 0;
}

silbot_parking::~silbot_parking()
{
  if ((int)m_laser_data.size() > 0)
    m_laser_data.clear();
}

void silbot_parking::laser_data_SubscribeCallBack(const sensor_msgs::LaserScan::ConstPtr &input)
{

  double cur_secs = ros::Time::now().toSec();

  if (cur_secs - old_secs > 0.2)
  {
    if ((int)m_laser_data.size() > 0)
      m_laser_data.clear();

    vector<eun_u::Point3D> start_point;
    vector<eun_u::Point3D> end_point;
    vector<eun_u::Point3D> position;
    vector<eun_u::Point3D> human_end;
    vector<eun_u::Point3D> human_start;
    vector<eun_u::Point3D> cur_vector;
    vector<eun_u::Point3D> curpos;
    vector<eun_u::Point3D> befopos;

    eun_u::Point3D temp_start_point;
    eun_u::Point3D temp_end_point;
    eun_u::Point3D temp_point;

    if ((int)start_point.size() > 0)
      start_point.clear();
    if ((int)end_point.size() > 0)
      end_point.clear();
    if ((int)position.size() > 0)
      position.clear();
    if ((int)human_start.size() > 0)
      human_start.clear();
    if ((int)human_end.size() > 0)
      human_end.clear();
    if ((int)cur_vector.size() > 0)
      cur_vector.clear();

    memset(&temp_start_point, 0, sizeof(eun_u::Point3D));
    memset(&temp_end_point, 0, sizeof(eun_u::Point3D));

    double angle_min = input->angle_min;
    double angle_increment = input->angle_increment;

    for (int i = 0; i < (int)input->ranges.size(); i++)
    {
      eun_u::Point3D temp_point;
      temp_point.x = input->ranges[i] * cos(angle_min + (angle_increment * i));
      temp_point.y = input->ranges[i] * sin(angle_min + (angle_increment * i));
      temp_point.z = 0.0;

        if (temp_point.x > 0.05 && temp_point.x < 0.50 && temp_point.y > -0.25 && temp_point.y < 0.25)
      {
        m_laser_data.push_back(temp_point);
      }

      else
      {
        temp_point.init();
        m_laser_data.push_back(temp_point);
      }
    }

    for (int i = 0; i < (int)m_laser_data.size() - 1; i++)
    {
      if (temp_start_point.x == 0 && temp_start_point.y == 0 && m_laser_data[i].x != 0 && m_laser_data[i].y != 0)
      {
        temp_start_point = m_laser_data[i];
      }

      double distance = mat_cal.distance_BetweenPoint3DAndPoint3D(m_laser_data[i], m_laser_data[i + 1]);

      if (distance > 0.2)
      {
        temp_end_point = m_laser_data[i];
        start_point.push_back(temp_start_point);
        end_point.push_back(temp_end_point);
        temp_start_point.init();
        temp_end_point.init();
      }

    }

    object_detector(start_point, end_point, position);
    printf("Num Object : %d\tNum Human : %d\n", (int)start_point.size(), (int)position.size()); //

    std_msgs::Int32 a;

    if ((int)position.size() > 0)
    {
      ROS_INFO("Human!!");
      m_human_flag = true;
      a.data = 1;
    }

    else if (m_human_flag == true && (int)position.size() == 0)
    {

      ROS_INFO("Leave!!");
      a.data = 2;
      m_human_flag = false;
    }

    else
    {
      ROS_INFO("None");
      a.data = 0;
      m_human_flag = false;
    }
    this->detect.publish(a);

    //human_display(human_end, human_start, start_point, end_point, position);
    //axis_display();
    //object(start_point, end_point);
    //laser_point_display();

    printf("Before : %d\tCur : %d\n", (int)m_before_position.size(), (int)position.size());

    old_secs = cur_secs;
    m_before_position = position;
    ros::Duration(0.01).sleep();
  }
}

bool silbot_parking::positionC(vector<eun_u::Point3D> curpos, vector<eun_u::Point3D> befopos)
{
  vector<eun_u::Point3D> position;
  vector<eun_u::Vector3D> direction;
  if ((int)position.size() > 0)
    position.clear();
  if ((int)direction.size() > 0)
    direction.clear();

  for (int i = 0; i < (int)curpos.size(); i++)
  {
    int index = -100;
    double min_distance = 100000000;

    if ((int)befopos.size() > 0)
    {
      for (int j = 0; j < (int)befopos.size(); j++)
      {
        double distance = mat_cal.distance_BetweenPoint3DAndPoint3D(curpos[i], m_before_position[j]);
        if (distance < min_distance)
        {
          index = j;
          min_distance = distance;
        }
      }

      if (index >= 0)
      {
        eun_u::Vector3D temp_vector;
        temp_vector.X = curpos[i].x - m_before_position[index].x;
        temp_vector.Y = curpos[i].y - m_before_position[index].y;
        temp_vector.Z = 0;
        temp_vector = mat_cal.make_unitvector(temp_vector);

        direction.push_back(temp_vector);
        position.push_back(befopos[index]);
        befopos.erase(befopos.begin() + index);
      }
    }
  }

  bool flag = false;
  for (int i = 0; i < (int)position.size(); i++)
  {
    eun_u::Vector3D temp_vec[2];
    temp_vec[0].X = -m_before_position[i].x;
    temp_vec[0].Y = -m_before_position[i].y;
    temp_vec[0].Z = -m_before_position[i].z;
    temp_vec[0] = mat_cal.make_unitvector(temp_vec[0]);
    temp_vec[1] = direction[i];

    double angle = mat_cal.angle_BetweenAandBvector(temp_vec[0], temp_vec[1]);
    if (fabs(angle) <= 90)
    {
      flag = true;
    }
  }

  return true;
}

void silbot_parking::object_detector(vector<eun_u::Point3D> start_point, vector<eun_u::Point3D> end_point, vector<eun_u::Point3D> &position)
{
  vector<eun_u::Point3D> human_start;
  vector<eun_u::Point3D> human_end;
  if ((int)human_start.size() > 0)
    human_start.clear();
  if ((int)human_end.size() > 0)
    human_end.clear();
  if ((int)position.size() > 0)
    position.clear();

  for (int i = 0; i < (int)start_point.size(); i++)
  {
    double distance = mat_cal.distance_BetweenPoint3DAndPoint3D(start_point[i], end_point[i]);

    if (distance > 0.01 && distance < 1.0)
    {
      eun_u::Point3D temp_point;
      temp_point.x = (start_point[i].x + end_point[i].x) / 2;
      temp_point.y = (start_point[i].y + end_point[i].y) / 2;
      temp_point.z = (start_point[i].z + end_point[i].z) / 2;
      human_start.push_back(start_point[i]);
      human_end.push_back(end_point[i]);
      position.push_back(temp_point);
    }
  }

  /*
  for(int i = 0; i < (int) position.size(); i++)
  {
    double distance;
    distance = mat_cal.distance_BetweenPoint3DAndPoint3D(human_end[i], human_start[i + 1]);
    if(distance < 0.4)
    {
      eun_u::Point3D temp_point;
      temp_point.x = (human_start[i].x + human_end[i+1].x)/2;
      temp_point.y = (human_start[i].y + human_end[i+1].y)/2;
      temp_point.z = (human_start[i].z + human_end[i+1].z)/2;
      position[i] = temp_point;
      position.erase(position.begin()+i+1);
    }
  }
  */
}
//Rviz Display
void silbot_parking::axis_display()
{
  visualization_msgs::Marker x_axis, y_axis, z_axis;
  x_axis.header.frame_id = y_axis.header.frame_id = z_axis.header.frame_id = "/laser_link";
  x_axis.header.stamp = y_axis.header.stamp = z_axis.header.stamp = ros::Time::now();
  x_axis.ns = y_axis.ns = z_axis.ns = "Axis";
  x_axis.action = y_axis.action = z_axis.action = visualization_msgs::Marker::ADD;
  x_axis.pose.orientation.w = y_axis.pose.orientation.w = z_axis.pose.orientation.w = 1.0;

  x_axis.id = 0;
  y_axis.id = 1;
  z_axis.id = 2;
  x_axis.type = y_axis.type = z_axis.type = visualization_msgs::Marker::LINE_LIST;

  // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
  x_axis.scale.x = y_axis.scale.x = z_axis.scale.x = 0.03;

  //X, Y, Z Axis Display
  geometry_msgs::Point p;
  x_axis.color.r = 1.0;
  x_axis.color.g = 0.0;
  x_axis.color.b = 0.0;
  x_axis.color.a = 1.0;

  y_axis.color.r = 0.0;
  y_axis.color.g = 1.0;
  y_axis.color.b = 0.0;
  y_axis.color.a = 1.0;

  z_axis.color.r = 0.0;
  z_axis.color.g = 0.0;
  z_axis.color.b = 1.0;
  z_axis.color.a = 1.0;

  p.x = 0.0;
  p.y = 0.0;
  p.z = 0.0;
  x_axis.points.push_back(p);
  p.x += 1.0;
  x_axis.points.push_back(p);

  p.x = 0.0;
  p.y = 0.0;
  p.z = 0.0;
  y_axis.points.push_back(p);
  p.y += 1.0;
  y_axis.points.push_back(p);

  p.x = 0.0;
  p.y = 0.0;
  p.z = 0.0;
  z_axis.points.push_back(p);
  p.z += 1.0;
  z_axis.points.push_back(p);

  m_pub_laser_data.publish(x_axis);
  m_pub_laser_data.publish(y_axis);
  m_pub_laser_data.publish(z_axis);
}

void silbot_parking::human_display(vector<eun_u::Point3D> human_end, vector<eun_u::Point3D> human_start, vector<eun_u::Point3D> start_point, vector<eun_u::Point3D> end_point, vector<eun_u::Point3D> &position)
{
  visualization_msgs::Marker human_r, human_b;
  human_r.header.frame_id = human_b.header.frame_id = "/laser_link";
  human_r.header.stamp = human_b.header.stamp = ros::Time::now();
  human_r.ns = human_b.ns = "human";
  human_r.action = human_b.action = visualization_msgs::Marker::ADD;

  eun_u::Point3D temp_point;

  human_r.pose.orientation.x = 0.0;
  human_r.pose.orientation.y = 0.0;
  human_r.pose.orientation.z = 0.0;
  human_r.pose.orientation.w = 1.0;

  human_b.pose.orientation.x = 0.0;
  human_b.pose.orientation.y = 0.0;
  human_b.pose.orientation.z = 0.0;
  human_b.pose.orientation.w = 1.0;

  human_r.type = human_b.type = visualization_msgs::Marker::LINE_LIST;

  human_r.scale.x = 0.02;
  human_r.scale.y = 0.02;
  human_r.scale.z = 0.02;

  human_b.scale.x = 0.02;
  human_b.scale.y = 0.02;
  human_b.scale.z = 0.02;

  human_r.id = 14;
  human_b.id = 15;

  human_r.color.r = 1.0f;
  human_r.color.g = 0.0f;
  human_r.color.b = 0.0f;
  human_r.color.a = 1.0f;

  human_b.color.r = 0.0f;
  human_b.color.g = 0.0f;
  human_b.color.b = 1.0f;
  human_b.color.a = 1.0f;

  geometry_msgs::Point start_p, end_p, human_s, human_e;
  for (int i = 0; i < (int)start_point.size() - 1; i++)
  {
    double distance = mat_cal.distance_BetweenPoint3DAndPoint3D(start_point[i], end_point[i]);
    if (distance > 0.05 && distance < 0.4)
    {

      temp_point.x = (start_point[i].x + end_point[i].x) / 2;
      temp_point.y = (start_point[i].y + end_point[i].y) / 2;
      temp_point.z = 0;

      start_p.x = start_point[i].x;
      start_p.y = start_point[i].y;
      start_p.z = 0;

      end_p.x = end_point[i].x;
      end_p.y = end_point[i].y;
      end_p.z = end_point[i].z;

      human_start.push_back(start_point[i]);
      human_end.push_back(end_point[i]);
      position.push_back(temp_point);
    }
    human_r.points.push_back(start_p);
    human_r.points.push_back(end_p);
  }

  for (int i = 0; i < (int)position.size(); i++)
  {

    double distance;
    distance = mat_cal.distance_BetweenPoint3DAndPoint3D(human_end[i], human_start[i + 1]);
    if (distance < 0.4)
    {
      //eun_u::Point3D temp_point;
      temp_point.x = (human_start[i].x + human_end[i + 1].x) / 2;
      temp_point.y = (human_start[i].y + human_end[i + 1].y) / 2;
      temp_point.z = (human_start[i].z + human_end[i + 1].z) / 2;

      human_s.x = human_start[i].x;
      human_s.y = human_start[i].y;
      human_s.z = 0;

      human_e.x = human_end[i + 1].x;
      human_e.y = human_end[i + 1].y;
      human_e.z = 0;

      position[i] = temp_point;
      position.erase(position.begin() + i + 1);
    }
    human_b.points.push_back(human_e);
    human_b.points.push_back(human_s);
  }

  m_pub_laser_data.publish(human_r);
  ros::Duration(0.005).sleep();
  m_pub_laser_data.publish(human_b);
}

void silbot_parking::object(vector<eun_u::Point3D> start_point, vector<eun_u::Point3D> end_point)
{
  visualization_msgs::Marker object_lines_r, object_lines_g;
  object_lines_r.header.frame_id = object_lines_g.header.frame_id = "/laser_link";
  object_lines_r.header.stamp = object_lines_g.header.stamp = ros::Time::now();
  object_lines_r.ns = object_lines_g.ns = "object_lines";
  object_lines_r.action = object_lines_g.action = visualization_msgs::Marker::ADD;

  object_lines_r.pose.orientation.x = 0.0;
  object_lines_r.pose.orientation.y = 0.0;
  object_lines_r.pose.orientation.z = 0.0;
  object_lines_r.pose.orientation.w = 1.0;

  object_lines_g.pose.orientation.x = 0.0;
  object_lines_g.pose.orientation.y = 0.0;
  object_lines_g.pose.orientation.z = 0.0;
  object_lines_g.pose.orientation.w = 1.0;

  object_lines_r.type = object_lines_g.type = visualization_msgs::Marker::LINE_LIST;

  object_lines_r.scale.x = 0.02;
  object_lines_r.scale.y = 0.02;
  object_lines_r.scale.z = 0.02;

  object_lines_g.scale.x = 0.02;
  object_lines_g.scale.y = 0.02;
  object_lines_g.scale.z = 0.02;

  object_lines_r.id = 10;
  object_lines_g.id = 12;

  object_lines_r.color.r = 1.0f;
  object_lines_r.color.g = 0.0f;
  object_lines_r.color.b = 0.0f;
  object_lines_r.color.a = 1.0f;

  object_lines_g.color.r = 0.0f;
  object_lines_g.color.g = 1.0f;
  object_lines_g.color.b = 0.0f;
  object_lines_g.color.a = 1.0f;

  geometry_msgs::Point start_p, end_p;
  for (int i = 0; i < (int)start_point.size() - 1; i++)
  {
    start_p.x = (float)start_point[i].x;
    start_p.y = (float)start_point[i].y;
    start_p.z = (float)0.0;

    end_p.x = (float)end_point[i].x;
    end_p.y = (float)end_point[i].y;
    end_p.z = (float)0.0;

    if (i % 3 == 0)
    {
      object_lines_r.points.push_back(start_p);
      object_lines_r.points.push_back(end_p);
    }

    else
    {
      object_lines_g.points.push_back(start_p);
      object_lines_g.points.push_back(end_p);
    }
  }

  m_pub_laser_data.publish(object_lines_r);
  ros::Duration(0.005).sleep();
  m_pub_laser_data.publish(object_lines_g);
}

void silbot_parking::laser_point_display()
{
  visualization_msgs::Marker points;
  points.header.frame_id = "/laser_link";
  points.header.stamp = ros::Time::now();
  points.action = visualization_msgs::Marker::ADD;

  points.pose.orientation.x = 0.0;
  points.pose.orientation.y = 0.0;
  points.pose.orientation.z = 0.0;
  points.pose.orientation.w = 1.0;

  points.ns = "Laser Data";

  points.type = visualization_msgs::Marker::POINTS;

  points.scale.x = 0.02;
  points.scale.y = 0.02;
  points.scale.z = 0.02;

  points.id = 3;

  points.color.r = 1.0f;
  points.color.a = 1.0f;

  geometry_msgs::Point p;
  for (int i = 0; i < m_laser_data.size(); i++)
  {
    if (m_laser_data[i].x != 0 && m_laser_data[i].y != 0)
    {
      p.x = (float)m_laser_data[i].x;
      p.y = (float)m_laser_data[i].y;
      p.z = (float)0.0;
    }
    points.points.push_back(p);
  }

  m_pub_laser_data.publish(points);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "silbot_parking");
  silbot_parking GGG;
  ros::spin();

  return (0);
}
