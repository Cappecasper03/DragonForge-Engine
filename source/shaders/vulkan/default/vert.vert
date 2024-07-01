#version 460 core

layout( location = 0 ) in vec4 inColor;
layout( location = 1 ) in vec3 inPosition;

layout( binding = 0 ) uniform UBO
{
	mat4 matrix;
}
ubo;

layout( location = 0 ) out vec3 outColor;

void main()
{
	gl_Position = ubo.matrix * vec4( inPosition, 1.0f );
	outColor    = inColor.rgb;
}