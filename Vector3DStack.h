#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Vector3DStack
{
public:
	Vector3DStack(float x, float y, float z);
	Vector3DStack();
	~Vector3DStack(void);
	float* GetCoordinates(void);

	float GetX(void);
	float GetY(void);
	float GetZ(void);
	float GetMagnitude(void);
	void Add(Vector3DStack vec);
	void Substract(Vector3DStack vec);
	void SetCoord(float x, float y, float z);
	void Multiply(float scalar);
	void Divide(float scalar);
	float ScalarProduct(Vector3DStack vec);
	Vector3DStack CrossProduct(Vector3DStack vec);

	Vector3DStack Vector3DStack::operator&(Vector3DStack& vec)
	{
		return CrossProduct(vec);
	}

	Vector3DStack Vector3DStack::operator+(Vector3DStack& vec)
	{
		Vector3DStack result;
		result.coord[0] = coord[0] + vec.coord[0];
		result.coord[1] = coord[1] + vec.coord[1];
		result.coord[2] = coord[2] + vec.coord[2];
		
		return result;
	}

	Vector3DStack Vector3DStack::operator-(Vector3DStack& vec)
	{
		Vector3DStack result;
		result.coord[0] = coord[0] - vec.coord[0];
		result.coord[1] = coord[1] - vec.coord[1];
		result.coord[2] = coord[2] - vec.coord[2];
		
		return result;
	}

	Vector3DStack Vector3DStack::operator/(float& scal)
	{
		Vector3DStack result;
		result.coord[0] = coord[0] / scal;
		result.coord[1] = coord[1] / scal;
		result.coord[2] = coord[2] / scal;

		return result;
	}

	Vector3DStack Vector3DStack::operator*(float& scal)
	{
		Vector3DStack result;
		result.coord[0] = coord[0] * scal;
		result.coord[1] = coord[1] * scal;
		result.coord[2] = coord[2] * scal;

		return result;
	}

	Vector3DStack Vector3DStack::operator=(Vector3DStack& vec)
	{
		if (&vec == this)
		{
			return *this;
		}

		coord[0] = vec.coord[0];
		coord[1] = vec.coord[1];
		coord[2] = vec.coord[2];

		return *this;
	}

	float Vector3DStack::operator*(Vector3DStack& vec)
	{
		return ScalarProduct(vec);
	}

	Vector3DStack GetUnitVector();
	Vector3DStack GetPerpendicularUnit(Vector3DStack vec);

private:
	float coord[3];
};

ostream& operator<<(ostream& os, Vector3DStack& vec);

