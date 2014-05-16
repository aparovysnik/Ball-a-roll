#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in  vec3 position;
in  vec2 texCoord;
in  vec4 colour;
in	vec3 normal;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
} OUT;



void main(void)	{
	gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	vec3 worldPos	= mat3(modelMatrix) * position;
	OUT.normal 		= mat3(modelMatrix) * normal;
	OUT.worldPos	= worldPos;
	OUT.texCoord	= texCoord;
	OUT.colour		= colour;
	
	OUT.colour = colour;
}