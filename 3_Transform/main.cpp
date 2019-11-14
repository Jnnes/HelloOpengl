#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <custom/shader.h>
#include <custom/camer.h>
#include <custom/Model.h>

#include <iostream>
#include <custom/log.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::string getResPath(std::string path);
unsigned int loadTexture(const char *path);
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

static const char TAG[] = "MAIN";

const char windowsTitles[] = "HelloTransform";

////定义函数原型
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////找到正确的函数并赋值给指针
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");

void key_callback(GLFWwindow*, int, int, int, int);
    
GLfloat opcity = 0.2f;
GLfloat vertices[] = {
    - 1.0f, 0.0f, 0.0f, // 第一个三角形
    -0.5f, 0.5f, 0.0f,
    0.0f, 0.0f, 0.0f, 
};
GLfloat vertices1[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 右上 // 手动在这里翻转Y轴纹理坐标
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // 左上
};

GLuint indices[] = {
    0,1,2,
};

GLuint indices1[] = {
    1,2,3,
    0,1,3,
};

GLfloat texCoords[] = {
    0.0f, 0.0f, // left bottom
    1.0f, 0.0f, // right bottom
    0.5f, 1.0f  // up center
};

int main(void) {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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

    // 开启多重采样缓冲（Multisample Buffer）
    glEnable(GL_MULTISAMPLE);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 从此刻起，我们使用的任何在GL_ARRAY_BUFFER目标上的缓冲调用都会用来配置当前VBO
    // 然后我们可以调用GLBufferData函数，他会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    GLuint EBO1;
    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    GLuint VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    // 从此刻起，我们使用的任何在GL_ARRAY_BUFFER目标上的缓冲调用都会用来配置当前VBO
    // 然后我们可以调用GLBufferData函数，他会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 禁用4字节对齐
    glActiveTexture(GL_TEXTURE0);
    GLuint texture = loadTexture("container.png");

    glActiveTexture(GL_TEXTURE1);
    GLuint texture1 = loadTexture("awesomeface.png");

    float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 转换纹理的操作完成后再开启4字节对齐
    
    Shader shader1("shader/shader1.vert", "shader/shader1.frag");
        
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    // 创建观察矩阵将顶点从世界坐标系转换到是相机坐标系
    // 这里只是模拟摄像机在[0， 0， 3]位置，所以该观察矩阵是将物体往屏幕里平移3个单位
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // 创建透视投影矩阵，将相机坐标系中的顶点转换到裁剪空间（平面，二维）
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);       

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT); //其他还有深度缓冲，模板缓冲

        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //
        // .. 绘制物体
        //glDrawArrays(GL_TRIANGLES, 0, 3);  

        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
        
        shader1.use();   
        glActiveTexture(GL_TEXTURE0); // 因为上面创建纹理时绑定了一次，但是修改完纹理后马上解绑了，所以这里必须重新绑定
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader1.ID, "ourTexture1"), 0);

        glActiveTexture(GL_TEXTURE1);// 因为上面创建纹理时绑定了一次，但是修改完纹理后马上解绑了，所以这里必须重新绑定
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader1.ID, "ourTexture2"), 1);

        GLfloat timeValue = (GLfloat)glfwGetTime();
        glUniform1f(glGetUniformLocation(shader1.ID, "angle"), 10 * timeValue); // 输入参数设置旋转角度

        glUniform1f(glGetUniformLocation(shader1.ID, "opcity"), opcity); // 按键控制的透明度

        glm::mat4 modeltemp;
        modeltemp = glm::rotate(model, glm::radians(timeValue * 5), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "model"), 1, GL_FALSE, glm::value_ptr(modeltemp));
        glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0));
        trans = glm::rotate(trans, glm::radians(3.0f * timeValue), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(1.2, 1.2, 1.2));
        glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "trans"),1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO1);        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        // 第二个箱子左上角缩放
        GLfloat scale = sin(timeValue) / 4 + 0.75;
        glm::mat4 trans1;
        trans1 = glm::translate(trans1, glm::vec3(-0.5f, 0.5f, 0.0f));
        trans1 = glm::scale(trans1, glm::vec3(scale, scale, scale));
        glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "trans"), 1, GL_FALSE, glm::value_ptr(trans1));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        // 
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


std::string getResPath(std::string path) {
    char *runPath = new char[256];
    _get_pgmptr(&runPath);
    std::string dirStr(runPath);

    while (dirStr.find('\\') != dirStr.npos) {
        size_t index = dirStr.find('\\');
        dirStr[index] = '/';
    }

    // 往上两级
    for (unsigned int i = 0; i < 2; i++) {
        size_t index = dirStr.find_last_of('/');
        if (index != dirStr.npos)
            dirStr = dirStr.substr(0, index);
        else
            break;
    }

    return dirStr + "/resources" + path;
}