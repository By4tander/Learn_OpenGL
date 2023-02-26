//
// Created by 贾奕人 on 2023/2/26.
//

#include <glad/glad.h>//一定要在GLFW之前包涵glad
#include <GLFW/glfw3.h>

#include<iostream>

//窗口大小改变时自动调整视口大小，下回调函数实现,在int中注册这个函数
void framebuffer_size_callback(GLFWwindow*window,int width, int height)
{
    glViewport(0,0,width,height);
}
//输入控制函数
void processInput(GLFWwindow *window)
{
    //查用户是否按下了返回键(Esc)
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //如果用户的确按下了返回键，
    // 我们将通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为
    // true的方法关闭GLFW
}

const char *vertexShaderSource ="#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0";

int main()
{
    //实例化glfw窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //MAC系统
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    //创建窗口
    GLFWwindow * window= glfwCreateWindow(800,600,"Homework02",NULL,NULL);
    if(window==NULL)
    {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return-1;
    }
    glfwMakeContextCurrent(window);//设置当前上下文

    //初始化GLAD，用来管理openGL指针，调用任何openGL函数前都必须初始化
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return-1;
    }

    //注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //把这个着色器源码附加到着色器对象上，然后编译它
    glCompileShader(vertexShader);

    //fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //link shaders
    unsigned int shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    float vertices[]={
            //first one
            0.0f,0.8f,0.0f,
            -0.5f,0.5f,0.0f,
            -0.3f,-0.5f,0.0f,
            //second one
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);//1是该VBO（显存）缓冲ID
    // bind the Vertex Array Object first,
    // then bind and set vertex buffer(s)
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);//绑定到GL_ARRAY_BUFFER
    //把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //渲染循环
    while(!glfwWindowShouldClose(window))//检测是否要求被退出,
    {
        //input
        processInput(window);

        //render
        //-----
        //迭代开始总是清屏幕
        glClearColor(0.4f, 0.7f, 0.3f, 0.6f);//状态设置函数
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,6);//VBO方法

        glfwSwapBuffers(window);//检查有没有触发什么事件
        glfwPollEvents();//交换颜色缓冲,它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
    }

    glfwTerminate();
    return 0;
}