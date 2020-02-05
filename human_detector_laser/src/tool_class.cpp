//TOOL CLASS V1.02

#include <tool_class.h>

using namespace eun_u;


tool_class::tool_class()
{

}

tool_class::~tool_class()
{

}

//Covariance and SVD
int tool_class::SVDwithCov(vector<Point3D> input, Vector3D* eigen_Vector, double *eigen_Value, Point3D& mean_point)
{
	double covMat[9];
	if(makeCovMat(input, mean_point, covMat) == ERROR) return ERROR;
	SVD(covMat, eigen_Vector, eigen_Value);

	//for(int i = 0; i < 9; i++)	printf("Cov[%d] : %lf\n", i, covMat[i]);
	//for(int i = 0; i < 3; i++)	printf("eigen_value[%d] : %lf\n", i, eigen_Value[i]);
	//for(int i = 0; i < 3; i++)	printf("eigen_vec[%d] X : %lf\tY : %lf\tZ : %lf\n", i, eigen_Vector[i].X,  eigen_Vector[i].Y,  eigen_Vector[i].Z);
	return NOT_ERROR;
}

int tool_class::makeCovMat(vector<Point3D> input, Point3D &mean, double* out_cov)
{
	int i;
	memset(out_cov, 0, sizeof(double) * 9);
	Point3D* submean = new Point3D[input.size()];
	if(submean == NULL) return ERROR;

	mean = cal_meanPoint3D(input);

	for(i=0; i<(int)input.size(); i++)
	{
		submean[i].x = input[i].x - mean.x;
		submean[i].y = input[i].y - mean.y;
		submean[i].z = input[i].z - mean.z;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[0] = out_cov[0] + submean[i].x * submean[i].x;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[1] = out_cov[1] + submean[i].x * submean[i].y;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[2] = out_cov[2] + submean[i].x * submean[i].z;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[3] = out_cov[3] + submean[i].y * submean[i].x;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[4] = out_cov[4] + submean[i].y * submean[i].y;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[5] = out_cov[5] + submean[i].y * submean[i].z;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[6] = out_cov[6] + submean[i].z * submean[i].x;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[7] = out_cov[7] + submean[i].z * submean[i].y;
	}

	for(i=0; i<(int)input.size(); i++)
	{
		out_cov[8] = out_cov[8] + submean[i].z * submean[i].z;
	}

	if(submean) { delete [] submean; submean = NULL; }
	return	NOT_ERROR;
}

void tool_class::SVD(double* covMat, Vector3D* eigen_Vector, double* eigen_Value)
{
	//CvMat mean_mat;
	CvMat cov_mat; CvMat SVD_mat; CvMat SVD_V_mat;
	double SVD[9];
	double SVD_V[9];

	cvInitMatHeader(&cov_mat, 3, 3, CV_64FC1, covMat);
	cvInitMatHeader(&SVD_mat, 3, 3, CV_64FC1, SVD);
	cvInitMatHeader(&SVD_V_mat, 3, 3, CV_64FC1, SVD_V);

	cvSVD(&cov_mat, &SVD_mat, NULL, &SVD_V_mat, CV_SVD_V_T);

	eigen_Value[0] = SVD_mat.data.db[0];
	eigen_Value[1] = SVD_mat.data.db[4];
	eigen_Value[2] = SVD_mat.data.db[8];

	eigen_Vector[0].X = SVD_V_mat.data.db[0];
	eigen_Vector[0].Y = SVD_V_mat.data.db[1];
	eigen_Vector[0].Z = SVD_V_mat.data.db[2];

	eigen_Vector[1].X = SVD_V_mat.data.db[3];
	eigen_Vector[1].Y = SVD_V_mat.data.db[4];
	eigen_Vector[1].Z = SVD_V_mat.data.db[5];

	eigen_Vector[2].X = SVD_V_mat.data.db[6];
	eigen_Vector[2].Y = SVD_V_mat.data.db[7];
	eigen_Vector[2].Z = SVD_V_mat.data.db[8];

	//for(int i = 0; i < 3; i++)	printf("eigen_value[%d] : %lf\n", i, eigen_Value[i]);
	//for(int i = 0; i < 3; i++)	printf("eigen_vec[%d] X : %lf\tY : %lf\tZ : %lf\n", i, eigen_Vector[i].X,  eigen_Vector[i].Y,  eigen_Vector[i].Z);
}

//mean point and median point
Point3D tool_class::cal_meanPoint3D(vector <Point3D> input)
{
	int i;
	Point3D mean;
	mean.x = 0;	mean.y = 0; mean.z = 0;

	for(i=0; i<(int)input.size(); i++)
	{
		mean.x = mean.x + input[i].x;
		mean.y = mean.y + input[i].y;
		mean.z = mean.z + input[i].z;
	}

	mean.x = mean.x / input.size();
	mean.y = mean.y / input.size();
	mean.z = mean.z / input.size();

	return
		mean;
}

Point3D tool_class::cal_medianPoint3D(vector <Point3D> input)
{
	Point3D median_point;
	int num_point = (int) input.size();
	int median_index = num_point/2;
	vector<double> array_x;
	vector<double> array_y;
	vector<double> array_z;

	for(int i = 0; i < num_point; i++)
	{
		array_x.push_back(input[i].x);
		array_y.push_back(input[i].y);
		array_z.push_back(input[i].z);
	}

	sort(array_x.begin(), array_x.end());
	sort(array_y.begin(), array_y.end());
	sort(array_z.begin(), array_z.end());

	median_point.x = array_x[median_index];
	median_point.y = array_y[median_index];
	median_point.z = array_z[median_index];

	if((int) array_x.size() > 0) array_x.clear();
	if((int) array_y.size() > 0) array_y.clear();
	if((int) array_z.size() > 0) array_z.clear();

	/*
	printf("\n");
	for(int i = 0; i < num_point; i++)
		printf("Point[%d] X : %0.3lf\t Y : %0.3lf\tZ : %0.3lf\n", i, array_x[i], array_y[i], array_z[i]);
	printf("\n");
	*/

	return median_point;
}

//calculate min and max value
bool tool_class::cal_MinAndMax_double(vector<double>& input_data, double& out_min, double& out_max)
{
	if((int) input_data.size() <= 0)
	{
		printf("[ERROR] function 'cal_MinAndMax' input is wrong!\n");
		return false;
	}

	vector<double> temp_data = input_data;

	sort(temp_data.begin(), temp_data.end());

	out_min = temp_data[0];
	out_max = temp_data[(int) input_data.size() - 1];

	if((int) temp_data.size() > 0 ) temp_data.clear();

	/*
	printf("\n");
	printf("Min Value : %0.3lf\tMax Value : %0.3lf\n", out_min, out_max);
	printf("\n");
	 */

	return true;
}

//Fitting Plane
Plane3D tool_class::fit_plane(vector<Point3D> input)
{
	Plane3D plane;
	double eigen_Value[3];
	Vector3D eigen_Vector[3];
	Point3D mean_point;

	memset(&mean_point, 0, sizeof(Point3D));
	memset(eigen_Value, 0, sizeof(double) *3);
	memset(eigen_Vector, 0, sizeof(Vector3D) *3);

	SVDwithCov( input, eigen_Vector, eigen_Value, mean_point);
	eigen_Vector[2] = make_unitvector(eigen_Vector[2]);

	plane.equ_a = eigen_Vector[2].X;
	plane.equ_b = eigen_Vector[2].Y;
	plane.equ_c = eigen_Vector[2].Z;
	plane.equ_d = -(plane.equ_a*mean_point.x + plane.equ_b*mean_point.y + plane.equ_c*mean_point.z);;

	plane.normal_vec = eigen_Vector[2];
	plane.center_position = mean_point;
	plane.points = input;

	//printf("Point Size : %d\n", (int)plane.points.size());
	//printf("mean Point X : %0.3lf\tY : %0.3lf\tZ : %0.3lf\n", mean_point.x, mean_point.y, mean_point.z);
	//printf("Normal_Vector X : %lf\tY : %lf\tZ : %lf\n", plane.normal_vec.X,  plane.normal_vec.Y,  plane.normal_vec.Z);

	return plane;
}

//Fitting Line
Line3D tool_class::fit_line(vector<Point3D> input)
{
	Line3D line;

	double eigen_Value[3];
	Vector3D eigen_Vector[3];
	Point3D mean_point;

	memset(&mean_point, 0, sizeof(Point3D));
	memset(eigen_Value, 0, sizeof(double) *3);
	memset(eigen_Vector, 0, sizeof(Vector3D) *3);

	SVDwithCov(input, eigen_Vector, eigen_Value, mean_point);

	line.mean_point = mean_point;
	line.direction_vector = make_unitvector(eigen_Vector[0]);
	line.points = input;

	return line;
}

//Distance between Point3D and Point3D
double tool_class::distance_BetweenPoint3DAndPoint3D(Point3D& input_point1, Point3D& input_point2)
{
	double point_distance;

	point_distance = pow((input_point1.x - input_point2.x), 2) + pow((input_point1.y - input_point2.y), 2) + pow((input_point1.z - input_point2.z), 2);
	point_distance = sqrt(point_distance);

	return point_distance;
}

//Distance between ZeroPoint3D and Point3D
double tool_class::distance_BetweenZeroPointAndPoint3D(Point3D& input_Point)
{
	double point_distance;

	point_distance = pow(input_Point.x, 2) + pow(input_Point.y, 2) + pow(input_Point.z, 2);
	point_distance = sqrt(point_distance);

	return point_distance;
}

//Distance between Plane and Point3D
double tool_class::distance_BetweenPlaneAndPoint3D(Plane3D& input_plane, Point3D& input_point)
{
	double Variable0, Variable1, Variable2, Variable3;
	double return_data;

	Variable0 = input_plane.equ_a*input_point.x + input_plane.equ_b*input_point.y + input_plane.equ_c*input_point.z + input_plane.equ_d;
	Variable1 = pow(input_plane.equ_a, 2) + pow(input_plane.equ_b, 2) + pow(input_plane.equ_c, 2);
	Variable2 = fabs(Variable0);
	Variable3 = sqrt(Variable1);
	return_data = Variable2/Variable3;

	return return_data;
}

//Make the unit vector
Vector3D tool_class::make_unitvector(Vector3D input)
{
	Vector3D unit_vector = input;

	double amp, outX, outY, outZ;
	outX = pow(unit_vector.X*1000, 2);
	outY = pow(unit_vector.Y*1000, 2);
	outZ = pow(unit_vector.Z*1000, 2);

	amp = sqrt(outX + outY + outZ)/1000;

	if(amp!=0)
	{
		unit_vector.X = unit_vector.X/amp;
		unit_vector.Y = unit_vector.Y/amp;
		unit_vector.Z = unit_vector.Z/amp;
	}
	else
	{
		unit_vector.X = 0;
		unit_vector.Y = 0;
		unit_vector.Z = 0;
	}

	//printf("Amp : %lf\n", amp);
	//printf("Unit_Vector X : %0.3lf\tY : %0.3lf\tZ : %0.3lf\n", unit_vector.X, unit_vector.Y, unit_vector.Z);
	return unit_vector;
}

//Vector DotProduct and CrossProduct(벡터의 내적, 외적)
double tool_class::DotProduct(Vector3D v1, Vector3D v2)
{
	double outValue;

	outValue = v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z;

	return	outValue;
}

Vector3D tool_class::CrossProduct(Vector3D start, Vector3D dest)
{
	Vector3D C;
	C.X = start.Y * dest.Z - start.Z * dest.Y;
	C.Y = start.Z * dest.X - start.X * dest.Z;
	C.Z = start.X * dest.Y - start.Y * dest.X;
	return C;
}

//Angle between A and B vector
double tool_class::angle_BetweenAandBvector(Vector3D A, Vector3D B)
{
	double angle;
	Vector3D unit_A, unit_B;

	unit_A = make_unitvector(A);
	unit_B = make_unitvector(B);

	angle = acos(DotProduct(unit_A, unit_B)) * RADIAN_TO_ANGLE;

	return angle;
}

//Rotation Vector
Vector3D tool_class::RoatedVectorAxisX(double Degree, Vector3D vec)
{
	double r_mat[9];
	double angle;
	Vector3D temp_Vec;

	angle = Degree/RADIAN_TO_ANGLE;

	r_mat[0] = 1;	r_mat[1] = 0;				r_mat[2] = 0;
	r_mat[3] = 0;	r_mat[4] = cos(angle);	r_mat[5] = -sin(angle);
	r_mat[6] = 0;	r_mat[7] = sin(angle);	r_mat[8] = cos(angle);

	temp_Vec.X = vec.X * r_mat[0] + vec.Y * r_mat[1] + vec.Z * r_mat[2];
	temp_Vec.Y = vec.X * r_mat[3] + vec.Y * r_mat[4] + vec.Z * r_mat[5];
	temp_Vec.Z = vec.X * r_mat[6] + vec.Y * r_mat[7] + vec.Z * r_mat[8];

	return temp_Vec;
}

Vector3D tool_class::RoatedVectorAxisY(double Degree, Vector3D vec)
{
	double r_mat[9];
	double angle;
	Vector3D temp_Vec;

	angle = Degree/RADIAN_TO_ANGLE;

	r_mat[0] = cos(angle);	r_mat[1] = 0;	r_mat[2] = sin(angle);
	r_mat[3] = 0;				r_mat[4] = 1;	r_mat[5] = 0;
	r_mat[6] = -sin(angle);	r_mat[7] = 0;	r_mat[8] = cos(angle);

	temp_Vec.X = vec.X * r_mat[0] + vec.Y * r_mat[1] + vec.Z * r_mat[2];
	temp_Vec.Y = vec.X * r_mat[3] + vec.Y * r_mat[4] + vec.Z * r_mat[5];
	temp_Vec.Z = vec.X * r_mat[6] + vec.Y * r_mat[7] + vec.Z * r_mat[8];

	return temp_Vec;
}

Vector3D tool_class::RoatedVectorAxisZ(double Degree, Vector3D vec)
{
	double r_mat[9];
	double angle;
	Vector3D temp_Vec;

	angle = Degree/RADIAN_TO_ANGLE;

	r_mat[0] = cos(angle);	r_mat[1] = -sin(angle);	r_mat[2] = 0;
	r_mat[3] = sin(angle);	r_mat[4] = cos(angle);	r_mat[5] = 0;
	r_mat[6] = 0;				r_mat[7] = 0;				r_mat[8] = 1;

	temp_Vec.X = vec.X * r_mat[0] + vec.Y * r_mat[1] + vec.Z * r_mat[2];
	temp_Vec.Y = vec.X * r_mat[3] + vec.Y * r_mat[4] + vec.Z * r_mat[5];
	temp_Vec.Z = vec.X * r_mat[6] + vec.Y * r_mat[7] + vec.Z * r_mat[8];

	return temp_Vec;
}

//Rotation Point
Point3D tool_class::RoatedPointAxisX(double Degree, Point3D point)
{
	double r_mat[9];
	double angle;
	Point3D temp_point;

	angle = Degree/RADIAN_TO_ANGLE;

	r_mat[0] = 1;	r_mat[1] = 0;				r_mat[2] = 0;
	r_mat[3] = 0;	r_mat[4] = cos(angle);	r_mat[5] = -sin(angle);
	r_mat[6] = 0;	r_mat[7] = sin(angle);	r_mat[8] = cos(angle);

	temp_point.x = point.x * r_mat[0] + point.y * r_mat[1] + point.z * r_mat[2];
	temp_point.y = point.x * r_mat[3] + point.y * r_mat[4] + point.z * r_mat[5];
	temp_point.z = point.x * r_mat[6] + point.y * r_mat[7] + point.z * r_mat[8];

	return temp_point;
}

Point3D tool_class::RoatedPointAxisY(double Degree, Point3D point)
{
	double r_mat[9];
	double angle;
	Point3D temp_point;

	angle = Degree/RADIAN_TO_ANGLE;

	r_mat[0] = cos(angle);	r_mat[1] = 0;	r_mat[2] = sin(angle);
	r_mat[3] = 0;				r_mat[4] = 1;	r_mat[5] = 0;
	r_mat[6] = -sin(angle);	r_mat[7] = 0;	r_mat[8] = cos(angle);

	temp_point.x = point.x * r_mat[0] + point.y * r_mat[1] + point.z * r_mat[2];
	temp_point.y = point.x * r_mat[3] + point.y * r_mat[4] + point.z * r_mat[5];
	temp_point.z = point.x * r_mat[6] + point.y * r_mat[7] + point.z * r_mat[8];

	return temp_point;
}

Point3D tool_class::RoatedPointAxisZ(double Degree, Point3D point)
{
	double r_mat[9];
	double angle;
	Point3D temp_point;

	angle = Degree/RADIAN_TO_ANGLE;

	r_mat[0] = cos(angle);	r_mat[1] = -sin(angle);	r_mat[2] = 0;
	r_mat[3] = sin(angle);	r_mat[4] = cos(angle);	r_mat[5] = 0;
	r_mat[6] = 0;				r_mat[7] = 0;				r_mat[8] = 1;

	temp_point.x = point.x * r_mat[0] + point.y * r_mat[1] + point.z * r_mat[2];
	temp_point.y = point.x * r_mat[3] + point.y * r_mat[4] + point.z * r_mat[5];
	temp_point.z = point.x * r_mat[6] + point.y * r_mat[7] + point.z * r_mat[8];

	return temp_point;
}



