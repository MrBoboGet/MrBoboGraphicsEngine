R"1337xd(#version 330
layout (location = 0) in vec3 VertexCoords;
layout (location = 1) in vec2 TextureCoords;

out vec2 FragTexCoord;
void main()
{
	gl_Position = vec4(VertexCoords,1);
	FragTexCoord = TextureCoords;
})1337xd"