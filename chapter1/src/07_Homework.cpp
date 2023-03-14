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
void init_glfw(){ glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}
auto create_window(int width, int height)->GLFWwindow * {
    //create window，must before the initialization of glad
    GLFWwindow *window = glfwCreateWindow(width, height, "07_Homework", NULL, NULL);
    glfwMakeContextCurrent(window);
    return window;
}
auto init_glad()->bool
{
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<<"GLAD faied"<<std::endl;
        return -1;
    }
}

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
#include "../include/texture.h"
int main()
{
    init_glfw();
    auto *window=create_window(WIDTH,HEIGHT);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    //always remember to initialize glad!
    init_glad();

    Shader shader(MY_SHADER_DIR + std::string("07shader_vertex.glsl"),
                  MY_SHADER_DIR + std::string("07shader_fragment.glsl"));

    unsigned int rectangle_VAO = pass_geometry_data_to_GPU(vertices, sizeof(vertices), indices, sizeof(indices));

    Texture texture1(MY_TEXTURE_DIR+std::string("emoji.jpg"));
    Texture texture2(MY_TEXTURE_DIR+std::string("wall.jpg"));

    //---------修改uniform一定要use-------
    shader.use();

    //非常重要，以下实现多个纹理单元的应用
    //glUniform1i(glGetUniformLocation(shader.ID,"texture1"),0);

    glUniform1i(glGetUniformLocation(shader.ID,"texture2"),1);

    //shader.set_int("texutre2",1);


    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //纹理单元操作，会默认激活0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1.getID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2.getID());

        //render container

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