#pragma once
#include "../nclgl/Vector3.h"

struct BoundingBox {
	BoundingBox()
	{
	}

	BoundingBox(Vector3 backBottomLeft, Vector3 frontTopRight)
	{
		this->backBottomLeft = backBottomLeft;
		this->frontTopRight = frontTopRight;
	}

	Vector3 backBottomLeft;
	Vector3 frontTopRight;
};

