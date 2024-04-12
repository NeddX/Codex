#shader_type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Colour;

out vec4 o_Colour;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main()
{
	o_Colour = a_Colour;
	gl_Position = u_Proj * u_View * vec4(a_Pos, 1.0);
}

#shader_type fragment
#version 330 core

in vec4 o_Colour;

out vec4 FragColour;

void main()
{
	FragColour = o_Colour;
}
