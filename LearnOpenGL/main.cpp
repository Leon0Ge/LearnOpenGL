#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//��������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//������ɫ�� ��ʱ Ӳ����(����ʱ��̬����Դ����)
const char *vertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Ƭ����ɫ�� ��ʱ Ӳ����
const char* fragShaderSource =  "#version 330 core \n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";

int main()
{
    // glfw:��ʼ������ �汾
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

    //----------------------------------------------------------------------��ã�������-------------------------------------------------------------------
    //������ɫ��
    unsigned int vertexShader;
    //������ɫ��
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //��ɫ��Դ���븽�ӵ���ɫ�������ϣ���ɫ������Դ���ַ���������ʵ��Դ�룩
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //������ɫ��
    glCompileShader(vertexShader);

    //�Ƿ�ɹ���־��
    int success;
    //������Ϣ����
    char infoLog[512];
    //�������Ƿ�ɹ�
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //��ȡ������Ϣ
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR  SHADER  VERTEX  COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Ƭ����ɫ��
    unsigned int fragShader;
    //������ɫ��
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //��ɫ��Դ���븽�ӵ���ɫ��������
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    //�༭��ɫ��
    glCompileShader(fragShader);
    //�������Ƿ�ɹ�
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //��ȡ������Ϣ
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR  SHADER  FRAGMENT  COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //��������ɫ���������ӵ�һ��������Ⱦ����ɫ������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //���������ɫ�����ӵ����������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    //���Ӷ���
    glLinkProgram(shaderProgram);
    //�������Ƿ�ɹ�
    glGetShaderiv(fragShader, GL_LINK_STATUS, &success);
    if (!success)
    {
        //��ȡ������Ϣ
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR  SHADER  LINK  COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //���������� ʹ����ɫ������
    //glUseProgram(shaderProgram);
    //ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    //�����εı�׼���豸����
    float vertices[] = {
       0.5f, 0.5f, 0.0f,        // ���Ͻ�
        0.5f, -0.5f, 0.0f,      // ���½�
        -0.5f, -0.5f, 0.0f,     // ���½�
        -0.5f, 0.5f, 0.0f       // ���Ͻ�
    };

    unsigned int indices[] = { // ע��������0��ʼ! 
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };
    //�����������
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //���嶥�㻺�����
    unsigned int VBO;
    //ͨ��һ������ID ����һ��VBO����
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //1.��VAO
    glBindVertexArray(VAO);
    //���㻺�����Ļ�������Ϊ GL_ARRAY_BUFFER �������Ļ����Ŀ����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���(�������ͣ��������ݴ�С��ʵ�����ݣ��Կ�����ʽ)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //ָ��OpenGL��ν��Ͷ�������(Location ͬ��Ӳ�����еĽ��������Ե�λ��ֵ��Ϊ0���������ԵĴ�С vec3����ֵ��ָ���������ͣ����ݱ�׼����������λ�������ڻ�������ʼλ�õ�ƫ����)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //�󶨺ͽ����֮�� ��glBufferData��������ֵ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#pragma ��������
    //unsigned int VAO;
    //glGenVertexArrays(1, &VAO);

    //1.��VAO
    //glBindVertexArray(VAO);
    //2.�Ѷ������鸳ֵ�������� ��OpenGLʹ�� �ظ�����
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //3.���ö�������ָ��
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // ʹ��һ�����㻺����󽫶������ݳ�ʼ���������У�������һ�������һ��Ƭ����ɫ����
    // ��������OpenGL��ΰѶ����������ӵ�������ɫ���Ķ��������ϡ�

    ////0.���ƶ������ݵ����������й�OpenGLʹ��
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    ////1.���ö�������ָ��
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    ////2.ʹ����ɫ������
    //glUseProgram(shaderProgram);
    ////3.��������
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
        //ˢ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //--------------------------------------------------------------------------����---------------------------------------------------------------------------

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        //----------------------------------------------------------------------------------------------------------------------------------------------------------
        // render
        // ------��Ⱦ�����ָ�������ˣ���������
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //ɾ������
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
