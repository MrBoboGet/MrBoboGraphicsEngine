#version 330
in vec2 FragTexCoord;
out vec4 FragColor;

uniform sampler2D RenderedImage;
uniform sampler2D DepthStencilTexture;
float LinearizeDepth(float Depth)
{
  float n = 0.1; // camera z near
  float f = 1000.0; // camera z far
  float z = Depth;
  return (2.0 * n) / (f + n - z * (f - n));	
}

float DepthTolerance = 0.02;
//beror egentligen på storleken av skärmen, men vi tar den konstant som det är nu
float XIncrement = 2.0/800.0;
float YIncrement = 2.0/600.0;
int IsBoundaryPoint(vec2 PointToCompare)
{
	//vi samplar i en cirkel så att
	PointToCompare.x = clamp(PointToCompare.x,0,1);
	PointToCompare.y = clamp(PointToCompare.y,0,1);
	if(PointToCompare.y-YIncrement < 0 || PointToCompare.y +YIncrement > 1 || PointToCompare.x-XIncrement < 0 || PointToCompare.x+XIncrement > 1)
	{
		return(0);
	}
	float PointToCompareDepth = LinearizeDepth(texture(DepthStencilTexture,PointToCompare).x);
	float RightD =	abs(LinearizeDepth(texture(DepthStencilTexture,FragTexCoord+vec2(XIncrement,0)).x)-PointToCompareDepth);
	float LeftD =	abs(LinearizeDepth(texture(DepthStencilTexture,FragTexCoord+vec2(-XIncrement,0)).x)-PointToCompareDepth);
	float UpD =		abs(LinearizeDepth(texture(DepthStencilTexture,FragTexCoord+vec2(0,YIncrement)).x)-PointToCompareDepth);
	float DownD =	abs(LinearizeDepth(texture(DepthStencilTexture,FragTexCoord+vec2(0,-YIncrement)).x)-PointToCompareDepth);
	if(RightD > DepthTolerance || LeftD > DepthTolerance || UpD > DepthTolerance || DownD > DepthTolerance)
	{
		return(1);
	}
	return(0);
}
int ShouldDrawLine()
{
	for(int i = 0; i < 2; i++)
	{
		int DrawLine = 0;
		DrawLine = IsBoundaryPoint(FragTexCoord+vec2(i*XIncrement,0));
		DrawLine = IsBoundaryPoint(FragTexCoord+vec2(-i*XIncrement,0));
		DrawLine = IsBoundaryPoint(FragTexCoord+vec2(0,i*YIncrement));
		DrawLine = IsBoundaryPoint(FragTexCoord+vec2(0,-i*YIncrement));
		if(DrawLine > 0)
		{
			return(1);
		}
	}
	return(0);
}
void main()
{
	//antar att vi samplar från 0
	float DepthValue = texture(DepthStencilTexture,FragTexCoord).x;
	DepthValue = LinearizeDepth(DepthValue);
	FragColor = vec4(texture(RenderedImage,FragTexCoord).rgb,1);
	if(ShouldDrawLine() != 0)
	{
		FragColor = vec4(0,0,0,1);
	}
	//FragColor += vec4(texture(RenderedImage,FragTexCoord).rgb,1)*vec4(0.001,0.001,0.001,1);
	//FragColor = vec4(1,1,0,1);
}