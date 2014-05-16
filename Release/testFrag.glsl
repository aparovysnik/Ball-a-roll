#version 150 core

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

uniform sampler2D tex0;

out vec4 out_col;

void main(void)	{
	out_col = IN.colour;
}