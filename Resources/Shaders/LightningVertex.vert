#version 330
layout (location = 0) in vec3 VerCoord;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 VertexTanget;
layout (location = 3) in vec3 VertexBitanget;
layout (location = 4) in vec3 VertexNormal;
//outs
out vec3 FragPos;
out vec2 TexPos;
out vec3 InNormal;
out VS_OUT
{
	mat3 TBN;
} Vs_Out;
out mat3 TBN;

//out vec3 Normal;
//geometry attributes
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalMatrix;
void main()
{
	gl_Position = Projection*View*Model*vec4(VerCoord,1);
	TexPos = TexCoord;
	FragPos = vec3(Model*vec4(VerCoord,1));
	//Normal = mat3(transpose(inverse(model))) * aNormal;  
	InNormal = mat3(NormalMatrix)*VertexNormal;
	vec3 T = normalize(mat3(NormalMatrix) * VertexTanget);
	vec3 B = normalize(mat3(NormalMatrix) * VertexBitanget);
	vec3 N = normalize(mat3(NormalMatrix) * VertexNormal);
	Vs_Out.TBN = mat3(T,B,N);
	TBN = mat3(T,B,N);
	//InNormal = mat3(transpose(inverse(Model)))*vec3(0,0,1);
}
