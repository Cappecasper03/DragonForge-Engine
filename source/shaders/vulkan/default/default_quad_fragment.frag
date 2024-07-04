#version 460 core

layout( push_constant ) uniform sPushConstant
{
	mat4 world_matrix;
	vec4 color;
}
PUSH_CONSTANT;

layout( location = 0 ) out vec4 out_color;

void main()
{
	out_color = PUSH_CONSTANT.color;
}