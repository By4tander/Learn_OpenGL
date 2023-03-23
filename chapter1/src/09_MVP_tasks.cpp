//
// Created by 贾奕人 on 2023/3/17.
//
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



void init_glfw();
auto init_glad() -> bool;
auto create_window(int width, int height) -> GLFWwindow *;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

#include "../include/shader.h"
#include "../include/texture.h"
#include "../include/renderable_object.h"

void trans()
{
    //定义一个向量，齐次坐标为1
    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);

    //创建4*4单位矩阵
    glm::mat4 trans=glm::mat4(1.0f);

    //创建4*4变幻矩阵，把单位矩阵和一个位移向量传给translate
    trans=glm::translate(trans,glm::vec3(1.0f,1.0f,0.0f));
    vec=trans*vec;
    //vec=vec*trans;
    std::cout<<vec.x<<vec.y<<vec.z<<std::endl;
}

auto main() -> int
{
    init_glfw();
    auto *window = create_window(800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    init_glad();

    Shader shader(MY_SHADER_DIR + std::string("09shader_vertex.glsl"), MY_SHADER_DIR + std::string("09shader_fragment.glsl"));

    std::array<float, 24> vertices = {
            // positions         // colors         // texture coords
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom left
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f   // top
    };
    std::array<unsigned int, 3> indices = {0, 1, 2};
    RenderableObject triangle(vertices.data(), sizeof(vertices), indices.data(), sizeof(indices), MY_TEXTURE_DIR + std::string("emoji.jpg"));

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // -------------------- NEW START --------------------
        // make sure to initialize matrix to identity matrix first！！
        glm::mat4 trans = glm::mat4(1.0f);

        //-------------------逆时针旋转90度-------------------
        //radians将角度转化为弧度，绕z轴旋转
        trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

        //缩放0.5倍
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

        unsigned int transformLocation=glGetUniformLocation(shader.ID,"transform");

        //以下函数，第二个参数告诉OpenGL要多少个矩阵，第三个参数询问是否需要转置。
        //openGL默认布局列主序(Column-major Ordering)，一般不需要转置
        //最后一个参数是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种
        // 因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
        glUniformMatrix4fv(transformLocation,1,GL_FALSE,glm::value_ptr(trans));
        //         pass transformation matrices to the shader
        //shader.set_mat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        //shader.set_mat4("view", view);

        // -------------------- NEW END --------------------

        triangle.render(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void init_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

auto init_glad() -> bool
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

auto create_window(int width, int height) -> GLFWwindow *
{
    GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



