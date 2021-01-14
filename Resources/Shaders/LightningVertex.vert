#version 330
layout (location = 0) in vec3 VerCoord;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 VertexTanget;
layout (location = 3) in vec3 VertexBitanget;
layout (location = 4) in vec3 VertexNormal;

layout (location = 5) in int BoneID1;
layout (location = 6) in float BoneWeight1;
layout (location = 7) in int BoneID2;
layout (location = 8) in float BoneWeight2;
layout (location = 9) in int BoneID3;
layout (location = 10) in float BoneWeight3;
layout (location = 11) in int BoneID4;
layout (location = 12) in float BoneWeight4;
layout (location = 13) in int BoneID5;
layout (location = 14) in float BoneWeight5;
struct LightSrc
{
	vec3 Color;
	vec3 WorldPos;
	float AmbStr;
	float SpecStr;
	float SpecExp;
};
//outs
out VS_OUT
{
	vec3 TangentFragPos;
	vec2 TexPos;
	vec3 TangentVertNormal;
	vec3 TangentViewPos;
	LightSrc TangentLights[4];
} Vs_Out;
//out vec3 Normal;
//geometry attributes
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalMatrix;
uniform vec3 ViewPos;
uniform LightSrc LightSources[4];
uniform mat4 BoneTransforms[70];
void main()
{
	//vi behöver också omvandla till bonespace
	mat4 TotalBoneTransformation = mat4(0);
	TotalBoneTransformation += BoneTransforms[BoneID1]*BoneWeight1;
	TotalBoneTransformation += BoneTransforms[BoneID2]*BoneWeight2;
	TotalBoneTransformation += BoneTransforms[BoneID3]*BoneWeight3;
	TotalBoneTransformation += BoneTransforms[BoneID4]*BoneWeight4;
	TotalBoneTransformation += BoneTransforms[BoneID5]*BoneWeight5;

	gl_Position = Projection*View*Model*TotalBoneTransformation*vec4(VerCoord,1);
	Vs_Out.TangentViewPos = ViewPos;
	Vs_Out.TexPos = TexCoord;
	Vs_Out.TangentFragPos = vec3(Model*TotalBoneTransformation*vec4(VerCoord,1));
	Vs_Out.TangentVertNormal = mat3(NormalMatrix)*mat3(TotalBoneTransformation)*VertexNormal;


	vec3 T = normalize(mat3(NormalMatrix) * VertexTanget);
	vec3 B = normalize(mat3(NormalMatrix) * VertexBitanget);
	vec3 N = normalize(mat3(NormalMatrix) * VertexNormal);
	//vi gör alla uträckningar itangent space
	mat3 ToTangentSpace = transpose(mat3(T,B,N));
	Vs_Out.TangentFragPos = ToTangentSpace*Vs_Out.TangentFragPos;
	Vs_Out.TangentVertNormal = ToTangentSpace*Vs_Out.TangentVertNormal;
	Vs_Out.TangentViewPos = ToTangentSpace*Vs_Out.TangentViewPos;
	for(int i = 0; i <4;i++)
	{
		Vs_Out.TangentLights[i] = LightSources[i];
		Vs_Out.TangentLights[i].WorldPos = ToTangentSpace*LightSources[i].WorldPos;
	}
}
