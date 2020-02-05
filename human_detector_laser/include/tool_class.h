//TOOL CLASS V1.02

#ifndef BKK_TOOL_CLASS
#define BKK_TOOL_CLASS

#include <iostream>
#include <string>
#include <math.h>
#include <vector>

//Open CV
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

namespace eun_u {

#define ERROR	1
#define NOT_ERROR 2
#define RADIAN_TO_ANGLE 57.2957795131

typedef struct Bounds3D
{
	double min_x;
	double max_x;
	double min_y;
	double max_y;
	double min_z;
	double max_z;

	Bounds3D()
	{
		init();
	}
	~Bounds3D()
	{

	}

	void init()
	{
		min_x = 0;
		min_y = 0;
		min_z = 0;
		max_x = 0;
		max_y = 0;
		max_z = 0;
	}
}Bounds3D;

typedef struct Image_resolution
{
	int image_width;
	int image_height;

	Image_resolution()
	{
		init();
	}
	~Image_resolution()
	{

	}

	void init()
	{
		image_width = 0;
		image_height = 0;
	}
}Image_resolution;

typedef struct PointUV
{
	int u;
	int v;

	PointUV()
	{
		init();
	}
	~PointUV()
	{

	}

	void init()
	{
		u = 0;
		v = 0;
	}
}PointUV;

typedef struct Point3D
{
	double x;
	double y;
	double z;

	Point3D()
	{
		init();
	}
	~Point3D()
	{

	}

	void init()
	{
		x = 0;
		y = 0;
		z = 0;
	}
}Point3D;

typedef struct Vector3D
{
	double X;
	double Y;
	double Z;
	Vector3D()
	{
		init();
	}
	~Vector3D()
	{

	}

	void init()
	{
		X = 0;
		Y = 0;
		Z = 0;
	}
}Vector3D;

typedef struct Object3D
{
	Point3D center_position;
	vector<Point3D> points;
	Object3D()
	{
		init();
	}
	~Object3D()
	{
		if((int) points.size() > 0) points.clear();
	}

	void init()
	{
		center_position.x = 0;
		center_position.y = 0;
		center_position.z = 0;
		if((int) points.size() > 0) points.clear();
	}
}Object3D;

typedef struct Plane3D
{
	double equ_a;
	double equ_b;
	double equ_c;
	double equ_d;

	Vector3D normal_vec;
	Point3D center_position;
	vector<Point3D> points;

	Plane3D()
	{
		init();
	}
	~Plane3D()
	{
		if((int) points.size() > 0) points.clear();
	}

	void init()
	{
		equ_a = 0;
		equ_b = 0;
		equ_c = 0;
		equ_d = 0;
		normal_vec.X = 0;
		normal_vec.Y = 0;
		normal_vec.Z = 0;
		center_position.x = 0;
		center_position.y = 0;
		center_position.z = 0;
		if((int) points.size() > 0) points.clear();
	}
}Plane3D;

typedef struct Line3D
{
	Vector3D direction_vector;
	Point3D mean_point;
	vector<Point3D> points;

	Line3D()
	{
		init();
	}
	~Line3D()
	{
		if((int) points.size() > 0) points.clear();
	}

	void init()
	{
		direction_vector.X = 0;
		direction_vector.Y = 0;
		direction_vector.Z = 0;
		mean_point.x = 0;
		mean_point.y = 0;
		mean_point.z = 0;
		if((int) points.size() > 0) points.clear();
	}
}Line3D;

typedef struct People3D
{
	Point3D center_position;
	double height;

	People3D()
	{
		init();
	}

	~People3D()
	{

	}

	void init()
	{
		center_position.x = 0;
		center_position.y = 0;
		center_position.z = 0;
		height = 0;
	}
}People3D;

typedef struct Tracking3D
{
	int id;
	Point3D center_position;
	double size_x;
	double size_y;
	double size_z;

	Tracking3D()
	{
		init();

	}

	~Tracking3D()
	{

	}

	void init()
	{
		center_position.x = 0;
		center_position.y = 0;
		center_position.z = 0;
		id = -1;
		size_x = 0;
		size_y = 0;
		size_z = 0;
	}
}Tracking3D;

class tool_class
{
public:
	tool_class();
	virtual ~tool_class();

	//Covariance and SVD
	int SVDwithCov(vector<Point3D> input, Vector3D* eigen_Vector, double* eigen_Value, Point3D& mean_point);
	int makeCovMat(vector<Point3D> input, Point3D &mean, double* out_cov);
	void CalSubMeanMatrix(vector <Point3D> input, Point3D &mean, Point3D* outmat);
	void SVD(double* covMat, Vector3D* eigen_Vector, double* eigen_Value);

	//mean point and median point
	Point3D cal_meanPoint3D(vector <Point3D> input);
	Point3D cal_medianPoint3D(vector <Point3D> input);

	//calculate min and max value
	bool cal_MinAndMax_double(vector<double>& input_data, double& out_min, double& out_max);

	//Fitting Plane
	Plane3D fit_plane(vector<Point3D> input);

	//Fitting Line
	Line3D fit_line(vector<Point3D> input);

	//Distance between Plane and Point3D
	double distance_BetweenPoint3DAndPoint3D(Point3D& input_point1, Point3D& input_point2);

	//Distance between ZeroPoint3D and Point3D
	double distance_BetweenZeroPointAndPoint3D(Point3D& input_Point);

	//Distance between Plane and Point3D
	double distance_BetweenPlaneAndPoint3D(Plane3D& input_plane, Point3D& input_point);

	//Make the unit vector
	Vector3D make_unitvector(Vector3D input);

	//Vector DotProduct and CrossProduct(벡터의 내적, 외적)
	double DotProduct(Vector3D v1, Vector3D v2);
	Vector3D CrossProduct(Vector3D start, Vector3D dest);

	//Angle between A and B vector
	double angle_BetweenAandBvector(Vector3D A, Vector3D B);

	//Rotation Vector
	Vector3D RoatedVectorAxisX(double Degree, Vector3D vec);
	Vector3D RoatedVectorAxisY(double Degree, Vector3D vec);
	Vector3D RoatedVectorAxisZ(double Degree, Vector3D vec);

	//Rotation Point
	Point3D RoatedPointAxisX(double Degree, Point3D point);
	Point3D RoatedPointAxisY(double Degree, Point3D point);
	Point3D RoatedPointAxisZ(double Degree, Point3D point);
};
}


#endif
