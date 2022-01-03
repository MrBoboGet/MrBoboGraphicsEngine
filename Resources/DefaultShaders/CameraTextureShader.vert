R"1337xd(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexPos;
//
//uniform mat4 TransformationMatrix;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 ViewPos;

out vec2 TexCoord;
void main()
{
    gl_Position = Projection*View*Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = TexPos;
})1337xd"