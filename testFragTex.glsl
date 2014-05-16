#version 150 core

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

uniform sampler2D tex0;

out vec4 out_col;

void main(void)	{
	out_col = vec4(1,1,1,texture(tex0,IN.texCoord).r) * IN.colour;
	
	//out_col = IN.colour;
}