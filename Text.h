#pragma once
#include "RenderObject.h"
#include "LFont.h"

using namespace std;
class Text : public RenderObject
{
public:
	Text(s);
	~Text();

private:
	vector<RenderObject*> text;
	LFont font;
};

