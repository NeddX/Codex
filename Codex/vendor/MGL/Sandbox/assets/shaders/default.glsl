#shader_type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Colour;
layout (location = 2) in vec2 a_TexCord;

out vec4 o_Colour;
out vec2 o_TexCord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	o_Colour = a_Colour;
	o_TexCord = a_TexCord;
	gl_Position = u_Proj * u_View * u_Model * vec4(a_Pos, 1.0);
}

#shader_type fragment
#version 330 core

out vec4 FragColour;

in vec2 o_TexCord;
in vec4 o_Colour;

uniform sampler2D u_Texture;
uniform float u_Blend;

void main()
{
	FragColour = texture(u_Texture, o_TexCord);
}