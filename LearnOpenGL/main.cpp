#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//窗口设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器 暂时 硬编码(运行时动态编译源代码)
const char *vertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//片段着色器 暂时 硬编码
const char* fragShaderSource =  "#version 330 core \n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";

int main()
{
    // glfw:初始化配置 版本
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //----------------------------------------------------------------------你好，三角形-------------------------------------------------------------------
    //顶点着色器
    unsigned int vertexShader;
    //创建着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //着色器源代码附加到着色器对象上（着色器对象，源码字符串数量，实际源码）
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //编译着色器
    glCompileShader(vertexShader);

    //是否成功标志量
    int success;
    //错误信息容器
    char infoLog[512];
    //检查编译是否成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //获取错误消息
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR  SHADER  VERTEX  COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //片段着色器
    unsigned int fragShader;
    //创建着色器
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //着色器源代码附加到着色器对象上
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    //编辑着色器
    glCompileShader(fragShader);
    //检查编译是否成功
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //获取错误消息
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR  SHADER  FRAGMENT  COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //将两个着色器对象链接到一个用来渲染的着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //将编译的着色器附加到程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    //链接对象
    glLinkProgram(shaderProgram);
    //检查编译是否成功
    glGetShaderiv(fragShader, GL_LINK_STATUS, &success);
    if (!success)
    {
        //获取错误消息
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR  SHADER  LINK  COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //激活程序对象 使用着色器程序
    //glUseProgram(shaderProgram);
    //删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    //三角形的标准化设备坐标
    float vertices[] = {
       0.5f, 0.5f, 0.0f,        // 右上角
        0.5f, -0.5f, 0.0f,      // 右下角
        -0.5f, -0.5f, 0.0f,     // 左下角
        -0.5f, 0.5f, 0.0f       // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    //索引缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //定义顶点缓冲对象
    unsigned int VBO;
    //通过一个缓冲ID 生成一个VBO对象
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //1.绑定VAO
    glBindVertexArray(VAO);
    //顶点缓冲对象的缓冲类型为 GL_ARRAY_BUFFER 将创建的缓冲绑到目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //将之前定义的顶点数据复制到缓冲的内存中(缓冲类型，传输数据大小，实际数据，显卡管理方式)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //指定OpenGL如何解释顶点数据(Location 同上硬编码中的将顶点属性的位置值设为0，顶点属性的大小 vec3三个值，指定数据类型，数据标准化，步长，位置数据在缓冲中起始位置的偏移量)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //绑定和解绑函数之间 用glBufferData把索引赋值到缓冲里
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#pragma 流程梳理
    //unsigned int VAO;
    //glGenVertexArrays(1, &VAO);

    //1.绑定VAO
    //glBindVertexArray(VAO);
    //2.把顶点数组赋值进缓冲中 供OpenGL使用 重复代码
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //3.设置顶点属性指针
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // 使用一个顶点缓冲对象将顶点数据初始化至缓冲中，建立了一个顶点和一个片段着色器，
    // 并告诉了OpenGL如何把顶点数据链接到顶点着色器的顶点属性上。

    ////0.复制顶点数据到缓冲数组中供OpenGL使用
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    ////1.设置顶点属性指针
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    ////2.使用着色器程序
    //glUseProgram(shaderProgram);
    ////3.绘制物体
    ////OpenGLFunctionThatDrawsOurTriangle();
#pragma endregion
    // 
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        //刷新数据
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //--------------------------------------------------------------------------绘制---------------------------------------------------------------------------

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        //----------------------------------------------------------------------------------------------------------------------------------------------------------
        // render
        // ------渲染出来又给我清掉了，锅在这里
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //删除数据
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
