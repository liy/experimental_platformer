#version 330

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

out vec4 ex_Color;

void main(void)
{
	gl_Position = projection_matrix * model_view_matrix * in_Position;

	ex_Color = in_Color;
}