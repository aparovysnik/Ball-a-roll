#version 150 core
uniform vec3 lightColour[10];
uniform vec3 lightPos[10];
uniform float lightRadius[10];

in Vertex	{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
} IN;

uniform sampler2D tex0;

out vec4 out_col;

void main(void)	{
	vec4 tmpCol = texture(tex0, IN.texCoord);
	vec3 normal = normalize(IN.normal);
	
	for (int i = 0; i < 10; i++)
	{
		float dist = length(IN.worldPos - lightPos[i]);
		vec3 direction = normalize(IN.worldPos - lightPos[i]);
		float atten = 1.0 - clamp (dist / lightRadius[i], 0.0, 1.0);
		tmpCol.xyz += lightColour[i] * atten * dot(normal,0-direction);
	}
	out_col = tmpCol;
}