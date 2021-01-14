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
uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D SpecTex;
uniform FragmentUniforms FragUni;
uniform LightSrc LightSources[4];
//in:s
in VS_OUT
{
	vec3 TangentFragPos;
	vec2 TexPos;
	vec3 TangentVertNormal;
	vec3 TangentViewPos;
	LightSrc TangentLights[4];
} Frag_In;

//lightning attributes
vec3 ApplyAmbiance(LightSrc Light);
vec3 ApplyDiffuse(LightSrc Light,vec3 Normal);
vec3 ApplySpecular(LightSrc Light,vec3 Normal);
//geometry attributes
vec3 FragNormal;


void main()
{
	//vi gör alla uträckningar i tangentspace
	if(FragUni.UseNormTex)
	{
		vec3 Temp = texture(NormalTex,Frag_In.TexPos).rgb;
		Temp = normalize(vec3(Temp.r,1-Temp.g,Temp.b)*2-1);
		FragNormal = normalize((vec3(Temp.x,Temp.y,Temp.z)));
	}
	else
	{
		FragNormal = normalize(Frag_In.TangentVertNormal);
	}
	vec3 SpecValue = vec3(0,0,0);
	if(FragUni.UseSpecTex)
	{
		SpecValue = vec3(texture(SpecTex,Frag_In.TexPos)).rrr;
	}
	else
	{
		SpecValue = vec3(1,1,1);
	}
	vec3 LightningColor = vec3(0,0,0);
	vec3 DiffuseColor = vec3(texture(DiffuseTex,Frag_In.TexPos));
	LightningColor += ApplyAmbiance(Frag_In.TangentLights[0]);
	LightningColor += ApplyDiffuse(Frag_In.TangentLights[0],FragNormal);
	LightningColor += SpecValue*ApplySpecular(Frag_In.TangentLights[0],FragNormal);
	
	FragColor = vec4(LightningColor*DiffuseColor,1);
	//bullshit så uniformen blir valid
	if(FragUni.UseDiffTex != 1)
	{
		FragColor = vec4(1,1,1,1);
		FragColor = vec4(LightSources[0].WorldPos,1);
	}
}


vec3 ApplyAmbiance(LightSrc Light)
{
	vec3 ReturnValue = Light.Color*Light.AmbStr;
	return(ReturnValue);
}
vec3 ApplyDiffuse(LightSrc Light,vec3 Normal)
{
	//kan veriera bereonde på lightsourcen, nu tar vi det enklaste fallet, point Light
	vec3 LightDir = normalize(Frag_In.TangentFragPos - Light.WorldPos);
	vec3 ReturnValue = max(dot(-LightDir,Normal),0)*Light.Color;
	return(ReturnValue);
}
vec3 ApplySpecular(LightSrc Light,vec3 Normal)
{
    vec3 LightDir = normalize(Frag_In.TangentFragPos-Light.WorldPos);
	vec3 ViewDir = normalize(Frag_In.TangentFragPos-Frag_In.TangentViewPos);
	vec3 ReflectDir = reflect(LightDir,Normal);
	float Spec = pow(max(dot(ReflectDir,-ViewDir),0),Light.SpecExp);
	vec3 ReturnValue = Spec*Light.Color*Light.SpecStr;
	return(ReturnValue);
}