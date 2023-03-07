//
// Created by 贾奕人 on 2023/3/6.
//
//
// Created by 贾奕人 on 2023/2/26.
//Based on 02_Homework
//

#include <glad/glad.h>//一定要在GLFW之前包涵glad
#include <GLFW/glfw3.h>
#include<cmath>
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
                                "layout(location=1) in vec3 aColor;\n"
                                "uniform vec3 offset;\n"//用来设置位移量
                                "void main()\n"
                                "{\n"
                                "gl_Position=vec4(aPos+offset,1.0);"
                                //"   gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);\n"//inverse_triangle,03homework01
                                //"ourColor=aColor;"//ourcolor得到定点数据那里输入颜色
                                "}\0";
const char *fragmentShaderSource00 = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "uniform vec4 myColor;"
                                     "void main()\n"
                                     "{\n"
                                     "FragColor=myColor;\n"
                                     "}\n\0";

auto main()->int
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
    GLFWwindow * window= glfwCreateWindow(800,600,"Homework03",NULL,NULL);//name
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

    //fragment shader
    unsigned int fragmentShader00;
    fragmentShader00 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader00, 1, &fragmentShaderSource00, NULL);
    glCompileShader(fragmentShader00);

    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    unsigned int shaderProgram00=glCreateProgram();
    //unsigned int shaderProgram01=glCreateProgram();
    //link first shader to the object
    glAttachShader(shaderProgram00,vertexShader);
    glAttachShader(shaderProgram00,fragmentShader00);
    glLinkProgram(shaderProgram00);

    float vertices_withcolor[]={
            //position                      //color
            0.0f, 0.5f, 0.0f,  1.0f,0.0f,0.0f , // left
            -0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,// right
            0.5f, -0.5f, 0.0f,   0.0f,0.0f,1.0f

    };

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2,VAO);
    glGenBuffers(2,VBO);//1是该VBO（显存）缓冲ID
    // bind the Vertex Array Object first,
    // then bind and set vertex buffer(s)
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);//绑定到GL_ARRAY_BUFFER
    //把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_withcolor),vertices_withcolor,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram00);  //激活,一定要先激活再用uniform
    //渲染循环
    while(!glfwWindowShouldClose(window))//检测是否要求被退出,
    {
        //input
        processInput(window);

        //render
        //-----
        //迭代开始总是清屏幕
        glClearColor(1.0f, 1.0f, 1.0f, 0.6f);//状态设置函数
        glClear(GL_COLOR_BUFFER_BIT);


        //更新颜色
        float timeValue=glfwGetTime();
        float colorValue1=std::sin(timeValue)/2+0.3;
        float colorValue2=std::sin(timeValue+2*3.1415/3)/2+0.3;
        float colorValue3=std::sin(timeValue-2*3.1415/3)/2+0.3;
        int vertexColorLocation=glGetUniformLocation(shaderProgram00,"myColor");
        //glUniform4f(vertexColorLocation,1.0f,colorValue,1.0f,1.0f);
        glUniform4f(vertexColorLocation,colorValue1,colorValue2,colorValue3,1.0f);

        //change position
        float offsetValue=glfwGetTime();
        float xoffset=std::sin(offsetValue)/2;
        float yoffset=std::cos(offsetValue)/2;
        int offsetLocation=glGetUniformLocation(shaderProgram00,"offset");
        glUniform3f(offsetLocation,xoffset,yoffset,1.0f);

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES,0,3);//VBO方法


        glfwSwapBuffers(window);//检查有没有触发什么事件
        glfwPollEvents();//交换颜色缓冲,它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
    }

    glfwTerminate();
    return 0;
}