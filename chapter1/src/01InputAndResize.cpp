#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * 之前提到过的函数，我们都把他放在main函数下面，只留下一个函数声明在这里。
 * 新的第一次出现的函数，我们都会放在main函数前面。
 */
void init_glfw();
auto init_glad() -> bool;
auto create_window(int width, int height) -> GLFWwindow *;

/**
 * 一个回调函数：当窗口大小改变的时候，就会调用这个函数
 * 回调函数是需要注册到OpenGL里面的，所以我们需要在main函数里面注册一下。
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
 * 一个正常的函数，用来处理用户输入。
 */
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

auto main() -> int
{
    /**
     * 之前学习过的代码的注释，我们都不再重复了。
     * 每行新的代码，我们都会加上注释。
     */
    init_glfw();
    auto *window = create_window(800, 600);

    // -------------------- NEW START --------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 将回调函数注册进OpenGL
    // -------------------- NEW END --------------------

    init_glad();

    while (!glfwWindowShouldClose(window))
    {
        // -------------------- NEW START --------------------
        process_input(window); // 处理用户输入
        // -------------------- NEW END --------------------

        //---------------渲染循环区域--------------

        glClearColor(1.0f, 1.0f, 0.9f, 1.0f);//状态设置函数
        //设置GLclear清除的颜色缓冲。
        // 如此时表示蓝色红色都全部清除，黄色还剩0.1

        //调用glClear函数来清空屏幕的颜色缓冲
        // 它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
        // 可能的缓冲位有GL_COLOR_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);//状态使用函数


        //------------渲染循环结束区----------------

        //必须有，不然全黑没交换过颜色缓冲
        glfwSwapBuffers(window);//函数会交换颜色缓冲
        // （它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
        // 它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。

        //必须有，不然没窗口显示
        glfwPollEvents();//检查有没有触发事件（比如键盘输入、鼠标移动等）
        // 更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
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
    GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    return window;
}