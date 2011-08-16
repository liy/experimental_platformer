#version 330

in vec2 ex_TexCoord;
in vec4 ex_Color;

out vec4 out_Color;

uniform sampler2D texture;

void main(void)
{
	//out_Color = ex_Color;

	// * ex_Color is for tint
	out_Color = texture2D(texture, ex_TexCoord.st) * ex_Color;
}