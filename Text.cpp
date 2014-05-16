#include "Text.h"


Text::Text(string text, LFont font)
{
	this->font = font;
}


Text::~Text()
{
	for (unsigned i = 0; i < text.size(); i++)
	{
		delete text.at(i);
	}
}
