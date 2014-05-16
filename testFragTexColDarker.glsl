#version 150 core

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

uniform sampler2D tex0;

out vec4 out_col;

void main(void)	{

	vec4 texcol = texture(tex0, IN.texCoord) * 0.6;
	out_col = vec4(IN.texCoord.x,0,IN.texCoord.y,1);
	out_col = texcol;
}