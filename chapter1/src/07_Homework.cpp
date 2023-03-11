//
// Created by 贾奕人 on 2023/3/10.
//
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow*window,int width, int height);
void processInput(GLFWwindow*window);
auto pass_geometry_data_to_GPU(float vertices_array[], int vertices_array_size, unsigned int indices_array[], int indices_array_size) ->unsigned int ;


const unsigned int WIDTH=800;
const unsigned int HEIGHT=600;

float vertices[] = {
         //positions                    // colors                    // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left

//        // positions                    // colors                    // texture coords
//        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,// top left
//        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// bottom left
//        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
//        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f // top right
};


unsigned int indices[]={
        0,1,3,
        1,2,3

//        2,3,1
//        ,0,1,3
};


#include "../include/shader.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //create window，must before the initialization of glad
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    //always remember to initialize glad!
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<<"GLAD faied"<<std::endl;
        return -1;
    }



    Shader shader(MY_SHADER_DIR + std::string("07shader_vertex.glsl"),
                  MY_SHADER_DIR + std::string("07shader_fragment.glsl"));

    unsigned int rectangle_VAO = pass_geometry_data_to_GPU(vertices, sizeof(vertices), indices, sizeof(indices));

    //----------------------
    //load and create texture
    unsigned int texture1,texture2;
    //texture1
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    //wraping parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    //filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load
    int width,height, nrChannels;//nrchannels颜色通道数
    //以下函数用于反转y坐标，因为OpenGL的纹理坐标起点在于左下角. 而图像纹理坐标的起点在于左上角.
    stbi_set_flip_vertically_on_load(true);
    const std::string texturepath=MY_TEXTURE_DIR+std::string("wall.jpg");
    unsigned char *data=stbi_load(texturepath.c_str(),&width,&height,&nrChannels,0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }
    //free buffer
    stbi_image_free(data);

    //------------
    //texture2
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);

    //wraping parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    //filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load
    int widt,heigh,crChannels;//crchannels颜色通道数
    //以下函数用于反转y坐标，因为OpenGL的纹理坐标起点在于左下角. 而图像纹理坐标的起点在于左上角.
    stbi_set_flip_vertically_on_load(true);
    const std::string texturepath2=MY_TEXTURE_DIR+std::string("emoji.jpeg");
    unsigned char *data2=stbi_load(texturepath2.c_str(),&widt,&heigh,&crChannels,0);

    if(data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widt, heigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }
    stbi_image_free(data2);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    // or set it via the texture class
    shader.set_int("texture2", 1);//



    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //render container
        shader.use();
        glBindVertexArray(rectangle_VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

};



auto pass_geometry_data_to_GPU(float vertices_array[],
                                int vertices_array_size,
                                unsigned int indices_array[],
                                int indices_array_size) -> unsigned int {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_array_size, vertices_array, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_array_size, indices_array, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;

}

void processInput(GLFWwindow *window)
{
    //查用户是否按下了返回键(Esc)
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

void framebuffer_size_callback(GLFWwindow*window,int width, int height)
{
glViewport(0,0,width,height);
}