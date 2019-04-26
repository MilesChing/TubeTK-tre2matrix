#pragma once
#include <cstdio>
#include <cmath>
using namespace std;

const double eps = 1e-8;

struct Point3 { 
	double x, y, z;  
	Point3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {} 
	bool operator == (const Point3& p) const {
		return fabs(x - p.x) < eps && fabs(y - p.y) < eps && fabs(z - p.z) < eps;
	}
};   

typedef Point3 Vector3;  

Vector3 operator+(const Vector3& A, const Vector3& B) { 
	return Vector3(A.x + B.x, A.y + B.y, A.z + B.z); 
} 

Vector3 operator-(const Point3 & A, const Point3 & B) { 
	return Vector3(A.x - B.x, A.y - B.y, A.z - B.z); 
}

Vector3 operator*(const Vector3 & A, double p) { 
	return Vector3(A.x * p, A.y * p, A.z * p); 
} 

Vector3 operator/(const Vector3 & A, double p) { 
	return Vector3(A.x / p, A.y / p, A.z / p); 
}

int dcmp(double x) { 
	if (fabs(x) < eps) 
		return 0;  else  
	return x < 0 ? -1 : 1; 
}  

double Dot(const Vector3 & A, const Vector3 & B) { 
	return A.x* B.x + A.y * B.y + A.z * B.z; 
} 

double Length(const Vector3 & A) { 
	return sqrt(Dot(A, A)); 
} 

double Angle(const Vector3 & A, const Vector3 & B) { 
	return acos(Dot(A, B) / Length(A) / Length(B)); 
} 

Vector3 Cross(const Vector3 & A, const Vector3 & B) { 
	return Vector3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); 
} 

double DistanceToSegment(Point3 P, Point3 A, Point3 B) { 
	if (A == B) return Length(P - A); 
	Vector3 v1 = B - A, v2 = P - A, v3 = P - B; 
	if (dcmp(Dot(v1, v2)) < 0) 
		return Length(v2); 
	else if (dcmp(Dot(v1, v3)) > 0)
		return Length(v3);
	else return Length(Cross(v1, v2)) / Length(v1); 
}
