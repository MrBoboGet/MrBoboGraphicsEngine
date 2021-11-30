R"1337xd(#version 330
in vec2 FragTexCoord;
out vec4 FragColor;

uniform sampler2D RenderedImage;
void main()
{
	FragColor = vec4(texture(RenderedImage,FragTexCoord).rgb,1);
})1337xd"