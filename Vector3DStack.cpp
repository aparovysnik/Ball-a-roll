#include "StdAfx.h"
#include "Vector3DStack.h"
#include <vector>


Vector3DStack::Vector3DStack(float x, float y, float z)
{
	SetCoord(x,y,z);
}

Vector3DStack::Vector3DStack()
{
}


Vector3DStack::~Vector3DStack(void)
{
}

float* Vector3DStack::GetCoordinates(void)
{
	return coord;
}
void Vector3DStack::SetCoord(float x, float y, float z)
{
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
}
void Vector3DStack::Add(Vector3DStack vec)
{
	coord[0] += vec.coord[0];
	coord[1] += vec.coord[1];
	coord[2] += vec.coord[2];
}

void Vector3DStack::Substract(Vector3DStack vec)
{
	coord[0] -= vec.coord[0];
	coord[1] -= vec.coord[1];
	coord[2] -= vec.coord[2];
}

void Vector3DStack::Multiply(float scalar)
{
	coord[0] *= scalar;
	coord[1] *= scalar;
	coord[2] *= scalar;
}

void Vector3DStack::Divide(float scalar)
{
	coord[0] /= scalar;
	coord[1] /= scalar;
	coord[2] /= scalar;
}

float Vector3DStack::ScalarProduct(Vector3DStack vec)
{
	float prod = 0;
	for (int i = 0; i < 3; i++)
	{
		prod += coord[i] * vec.coord[i];
	}

	return prod;
}

Vector3DStack Vector3DStack::CrossProduct(Vector3DStack vec)
{
	Vector3DStack vector;
	vector.SetCoord((coord[1] * vec.coord[2]) - (coord[2] * vec.coord[1]),
		(coord[2] * vec.coord[0]) - (coord[0] * vec.coord[2]),
		(coord[0] * vec.coord[1]) - (coord[1] * vec.coord[0]));
	
	return vector;
}

Vector3DStack Vector3DStack::GetUnitVector()
{
	Vector3DStack vec;
	vec.SetCoord(this->GetX(), this->GetY(), this->GetZ());
	float mag = this->GetMagnitude();
	vec.Divide(mag);

	return vec;
}

Vector3DStack Vector3DStack::GetPerpendicularUnit(Vector3DStack vec)
{
	return this->CrossProduct(vec).GetUnitVector();
}

float Vector3DStack::GetX(void)
{
	return coord[0];
}

float Vector3DStack::GetY(void)
{
	return coord[1];
}

float Vector3DStack::GetZ(void)
{
	return coord[2];
}

float Vector3DStack::GetMagnitude(void)
{
	float r = sqrt((coord[0]*coord[0]) + (coord[1]*coord[1]) + (coord[2]*coord[2]));
	return r;
}

ostream& operator<<(ostream& os, Vector3DStack& vec)
{
	os << vec.GetX() << "x, " << vec.GetY() << "y, " << vec.GetZ() << "z";
	return os;
}

