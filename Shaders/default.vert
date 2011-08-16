#version 330

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_TexCoord;

uniform mat4 projection;
uniform mat4 modelView;

out vec4 ex_Color;
out vec2 ex_TexCoord;

void main(void)
{
	gl_Position = projection * modelView * in_Position;

	ex_Color = in_Color;
	ex_TexCoord = in_TexCoord;
}