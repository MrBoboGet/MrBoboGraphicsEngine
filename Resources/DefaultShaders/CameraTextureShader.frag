R"1337xd(#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D TextureToRender;
void main()
{
    FragColor = texture(TextureToRender,TexCoord);
    //FragColor = vec4(1,0,0,1);
})1337xd"