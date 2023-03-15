#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &texture_path)
{
    unsigned int texture_ID;
    glGenTextures(1, &texture_ID);

    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //环绕方式非常有趣，注意单一轴修改的效果
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nr_channels;

    //以下语句实现正面使用图像
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nr_channels, 0);
    if (data)
    {
        //format参数用来决定原图的格式
		auto format = GL_RED;
		if (nr_channels == 1)
			format = GL_RED;
		else if (nr_channels == 3)
			format = GL_RGB;
		else if (nr_channels == 4)
			format = GL_RGBA;

        //绑定纹理后，以下函数生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
        stbi_image_free(data);

    ID = texture_ID;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
//    //激活
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D,ID);
}

auto Texture::getID() -> unsigned int {
    return ID;
}