#version 330 core
struct LightSrc
{
	vec3 Color;
	vec3 WorldPos;
	float AmbStr;
	float SpecStr;
	float SpecExp;
};
struct FragmentUniforms
{
	int UseNormTex;
	int UseDiffTex;
	int UseSpecTex;
};
//outs
out vec4 FragColor;
//Uniforms
uniform vec3 ViewPos;
uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D SpecTex;
uniform LightSrc LightSources[4];
uniform FragmentUniforms FragUni;
//in:s
in VS_OUT
{
	mat3 TBN;
} Vs_Out;
in vec3 FragPos;
in vec2 TexPos;
in vec3 InNormal;
in mat3 TBN;
//lightning attributes
vec3 ApplyAmbiance(LightSrc Light);
vec3 ApplyDiffuse(LightSrc Light,vec3 Normal);
vec3 ApplySpecular(LightSrc Light,vec3 Normal);
//geometry attributes
vec3 FragNormal;
void main()
{
	//FragNormal =  normalize(texture(NormalTex,TexPos).rgb*2-1);
	if(FragUni.UseNormTex)
	{
		vec3 Temp = texture(NormalTex,TexPos).rgb;
		//FragNormal = Temp;
		Temp = normalize(vec3(Temp.r,1-Temp.g,Temp.b)*2-1);
		FragNormal = normalize(TBN*(vec3(Temp.x,Temp.y,Temp.z)));
	}
	else
	{
		FragNormal = normalize(InNormal);
	}
	vec3 SpecValue = vec3(0,0,0);
	if(FragUni.UseSpecTex)
	{
		SpecValue = vec3(texture(SpecTex,TexPos)).rrr;
	}
	else
	{
		SpecValue = vec3(1,1,1);
	}
	//FragNormal = normalize(InNormal);
	vec3 LightningColor = vec3(0,0,0);
	vec3 DiffuseColor = vec3(texture(DiffuseTex,TexPos));
	LightningColor += ApplyAmbiance(LightSources[0]);
	LightningColor += ApplyDiffuse(LightSources[0],FragNormal);
	LightningColor += SpecValue*ApplySpecular(LightSources[0],FragNormal);
	
	FragColor = vec4(LightningColor*DiffuseColor,1);
	//FragColor = texture(DiffuseTex,TexPos);
}
vec3 ApplyAmbiance(LightSrc Light)
{
	vec3 ReturnValue = Light.Color*Light.AmbStr;
	return(ReturnValue);
}
vec3 ApplyDiffuse(LightSrc Light,vec3 Normal)
{
	//kan veriera bereonde på lightsourcen, nu tar vi det enklaste fallet, point Light
	vec3 LightDir = normalize(FragPos - Light.WorldPos);
	vec3 ReturnValue = max(dot(-LightDir,Normal),0)*Light.Color;
	return(ReturnValue);
}
vec3 ApplySpecular(LightSrc Light,vec3 Normal)
{
    vec3 LightDir = normalize(FragPos-Light.WorldPos);
	vec3 ViewDir = normalize(FragPos-ViewPos);
	vec3 ReflectDir = reflect(LightDir,Normal);
	float Spec = pow(max(dot(ReflectDir,-ViewDir),0),Light.SpecExp);
	vec3 ReturnValue = Spec*Light.Color*Light.SpecStr;
	return(ReturnValue);
}