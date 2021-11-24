#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexPos;
//
struct LightSrc
{
	vec3 Color;
	vec3 WorldPos;
	float AmbStr;
	float SpecStr;
	float SpecExp;
};
//uniform mat4 TransformationMatrix;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 ViewPos;
uniform mat4 NormalMatrix;
uniform LightSrc LightSources[4];

out vec2 TexCoord;
void main()
{
    gl_Position = Projection*View*Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = TexPos;
}