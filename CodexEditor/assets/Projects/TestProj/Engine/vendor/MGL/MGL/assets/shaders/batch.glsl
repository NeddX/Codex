#shader_type vertex
#version 330 core

layout(location = 0) vec3 a_Pos;
layout(location = 1) vec4 a_Colour;
layout(location = 2) vec2 a_TexCoord;
layout(location = 3) float a_TexID;

out vec4 o_Colour;
out vec2 o_TexCoord;
out int o_TexID;

main()
{
	o_Colour = a_Colour;
	o_TexCoord = a_TexCoord;
	o_TexID = int(a_TexID);
	gl_Position = a_Pos;
}

#shader_type fragment
#version 330 core

in vec4 o_Colour;
in vec2 o_TexCoord;
int int o_TexID;

out vec4 FragColour;

uniform samplerID u_Textures[8];

void main()
{
	FragColour = texture(o_TexID, o_TexCoord);
}