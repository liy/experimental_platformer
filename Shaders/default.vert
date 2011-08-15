#version 330

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;

uniform mat4 projection;
uniform mat4 modelView;

out vec4 ex_Color;

void main(void)
{
	gl_Position = projection * modelView * in_Position;

	ex_Color = in_Color;
}