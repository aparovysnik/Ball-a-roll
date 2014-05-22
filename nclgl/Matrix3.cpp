#include "Matrix3.h"


Matrix3::Matrix3(void)
{
}


Matrix3::~Matrix3(void)
{
}

Matrix3::Matrix3(const Matrix4 &m4)
{
	for (int i = 0; i < 9; i++)	{
		values[i] = 0.0f;
	}
	for (int i = 0; i < 3; i++)
	{
		Vector4 row = m4.GetRow(i);
		SetRow(i, Vector3(row.x, row.y, row.z));
	}
	float* v = values;
}