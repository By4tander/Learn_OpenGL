#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

//uniform sampler2D ourTexture;

void main()
{
    //texture是GLSL内建的函数用来采样纹理颜色
    //参数一是纹理采样器，参数二是对应的纹理坐标
    //FragColor=texture(ourTexture,TexCoord);

    //以下写法是叠加两个纹理
   FragColor = mix(texture(texture1, TexCoord),texture(texture2,TexCoord),0.5);
}
