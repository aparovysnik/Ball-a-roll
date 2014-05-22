/*
Class:Vector4
Implements:
Author:Rich Davison
Description:VERY simple Vector4 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#pragma once
#include <btBulletDynamicsCommon.h>

class Vector4	{
public:
	Vector4(void) {
		x = y = z = w = 1.0f;
	}
	Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4(btQuaternion* btVec)
	{
		this->x = btVec->x();
		this->y = btVec->y();
		this->z = btVec->z();
		this->w = btVec->w();
	}
	long double	Length() const {
		/*long double doublex = x*x;
		long double doublez = z*z;
		long double doubley = y*y;
		long double sum = doublez + doublex;
		double long sum2 = sum + doubley;*/
		return sqrt((x*x) + (y*y) + (z*z) + (w*w));
	}

	void Normalise() {
		float length = (float)Length();

		if (length != 0.0f)	{
			length = 1.0f / length;
			x = x * length;
			y = y * length;
			z = z * length;
			w = w * length;
		}
	}

	Vector4	Inverse() const{
		return Vector4(-x, -y, -z, w);
	}

	~Vector4(void){}

	float x;
	float y;
	float z;
	float w;
};

