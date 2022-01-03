R"1337xd(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexPos;

uniform mat4 Model;

out vec2 TexCoord;

void main()
{
	gl_Position = Model*vec4(aPos,1);
	TexCoord = TexPos;
})1337xd"