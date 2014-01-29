#version 400

in vec2 vert_texcoord;

out vec4 out_Color;

uniform sampler2D g_Diffuse;

void main()
{
	float frag_color = texture(g_Diffuse, vert_texcoord).r;
	out_Color = vec4(frag_color,frag_color,frag_color, 1.0f);
}