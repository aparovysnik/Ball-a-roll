#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <btBulletDynamicsCommon.h>
#include "Vector4.h"
/*
Class:Vector3
Implements:
Author:Rich Davison
Description:VERY simple Vector3 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#include <cmath>
#include <iostream>

class Vector3	{
public:
	Vector3(void) {
		ToZero();
	}

	Vector3(const float x, const float y, const float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(btVector3* btVec)
	{
		this->x = btVec->x();
		this->y = btVec->y();
		this->z = btVec->z();
	}

	Vector3(Vector4 v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}

	Vector3(const float a) {
		this->x = a;
		this->y = a;
		this->z = a;
	}

	~Vector3(void){}

	float x;
	float y;
	float z;

	void			Normalise() {
		long double length = Length();

		if(length != 0.0f)	{
			length = 1.0f / length;
			x = x * (float)length;
			y = y * (float)length;
			z = z * (float)length;
		}
	}

	btVector3 ToBtVector() {
		return btVector3(x, y, z);
	}

	long double sqrt (long double x) const {
		if (x <= 0)
			return 0;       // if negative number throw an exception?
		int exp = 0;
		x = frexp(x, &exp); // extract binary exponent from x
		if (exp & 1) {      // we want exponent to be even
			exp--;
			x *= 2;
		}
		long double y = (1 + x) / 2; // first approximation
		long double z = 0;
		while (y != z) {    // yes, we CAN compare doubles here!
			z = y;
			y = (y + x / y) / 2;
		}
		return ldexp(y, exp / 2); // multiply answer by 2^(exp/2)
	}

	void		ToZero() {
		x = y = z = 0.0f;
	}

	long double	Length() const {
		/*long double doublex = x*x;
		long double doublez = z*z;
		long double doubley = y*y;
		long double sum = doublez + doublex;
		double long sum2 = sum + doubley;*/
		return sqrt((x*x)+(y*y)+(z*z));	
	}

	static Vector3 PlaneProjection(Vector3 src, Vector3 n)
	{
		return (src - n * (Dot(src,n)));
	}

	//Computes the angle between two vectors
	static float AngleBetween(const Vector3& a, const Vector3& b)
	{
		if ((a.Length() == 0) || (b.Length() == 0))
		{
			return 0;
		}
		double val = Dot(a, b) / (a.Length() * b.Length());
		if (val > 1)
		{
			val = 1;
		}
		else if (val < -1)
		{
			val = -1;
		}
		return acos(val) * 180.0 / M_PI;
	}

	void			Invert() {
		x = -x;
		y = -y;	
		z = -z;	
	}

	Vector3			Inverse() const{
		return Vector3(-x,-y,-z);
	}

	static float	Dot(const Vector3 &a, const Vector3 &b) {
		return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
	}

	static Vector3	Cross(const Vector3 &a, const Vector3 &b) {
		return Vector3((a.y*b.z) - (a.z*b.y) , (a.z*b.x) - (a.x*b.z) , (a.x*b.y) - (a.y*b.x));	
	}

	inline friend std::ostream& operator<<(std::ostream& o, const Vector3& v) {
		o << "Vector3(" << v.x << "," << v.y << "," << v.z <<")" << std::endl;
		return o;
	}

	inline Vector3  operator+(const Vector3  &a) const{
		return Vector3(x + a.x,y + a.y, z + a.z);
	}

	inline Vector3  operator-(const Vector3  &a) const{
		return Vector3(x - a.x,y - a.y, z - a.z);
	}

	inline Vector3  operator-() const{
		return Vector3(-x,-y,-z);
	}

	inline void operator+=(const Vector3  &a){
		x += a.x;
		y += a.y;
		z += a.z;
	}

	inline void operator-=(const Vector3  &a){
		x -= a.x;
		y -= a.y;
		z -= a.z;
	}

	inline Vector3  operator*(const float a) const{
		return Vector3(x * a,y * a, z * a);
	}

	inline Vector3  operator*(const Vector3  &a) const{
		return Vector3(x * a.x,y * a.y, z * a.z);
	}

	inline Vector3  operator/(const Vector3  &a) const{
		float divX = 0, divY = 0, divZ = 0;

		if (a.x != 0)
			divX = (x / a.x);
		if (a.y != 0)
			divY = (y / a.y);
		if (a.z != 0)
			divZ = (z / a.z);
		return Vector3(divX,divY, divZ);
	};

	inline Vector3  operator/(const float v) const{
		return Vector3(x / v,y / v, z / v);
	};

	inline bool	operator==(const Vector3 &A)const {return (A.x == x && A.y == y && A.z == z) ? true : false;};
	inline bool	operator!=(const Vector3 &A)const {return (A.x == x && A.y == y && A.z == z) ? false : true;};
};

