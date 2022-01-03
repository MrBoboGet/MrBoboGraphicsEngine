R"1337xd(#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D TextureToRender;

void main()
{
    FragColor = texture(TextureToRender,TexCoord);
    //FragColor = FragColor.bgra;
})1337xd"