#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D OurTexture;
void main()
{
    FragColor = texture(OurTexture,TexCoord);
    //FragColor = vec4(1,0,0,1);
} 