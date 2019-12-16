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

const char windowsTitles[] = "HelloTriangle";
struct TextureStruct
{
    GLuint id;
    GLuint width;
    GLuint height;
    TextureStruct(GLuint id, GLuint width, GLuint height) {
        this->id = id;
        this->width = width;
        this->height = height;
    }
};

////定义函数原型
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////找到正确的函数并赋值给指针
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
TextureStruct loadTexture(char const * path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow*, int, int, int, int);
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
GLfloat vertices[] = {
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,-0.5f, 0.5f, // 左上
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,0.5f,0.5f,// 右上
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -0.5f,-0.5f, // 左下
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f,0.5f,-0.5f // 右下
};

GLuint indices[] = {
    0,1,2,3
};

int main() {
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
        
    Shader flagShader("flag.vert", "flag.frag");

    TextureStruct textureStruct = loadTexture("flag.png");
    GLuint texture = textureStruct.id;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int colCount = 800; // X轴 被裁成几份
    int rowCount = 500;   // Y轴 被裁成几份

    // 保证渲染出来的图片横纵比与原图片一致
    float XDet = 1.0 * textureStruct.width/ textureStruct.height / colCount;
    float YDet = 1.0 / rowCount;

    // 创建MVP矩阵
    glm::mat4 model;
    model = glm::scale(model, glm::vec3(XDet, YDet, max(XDet, YDet)));

    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    view = glm::rotate(view, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
     
    GLfloat lastTime = -1.0;
    int count = 0;
    int det = 2;
    while (!glfwWindowShouldClose(window)) {
        GLfloat timeValue = (GLfloat)glfwGetTime();
        if (timeValue - lastTime > det) {            
            std::cout << "fps = " << to_string(count / det) << std::endl;
            count = 0;
            lastTime = timeValue;
        }
        count++;       

        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT); //其他还有深度缓冲，模板缓冲
        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //
        // .. 绘制物体
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        flagShader.setInt("ourTexture", 0);
        flagShader.setMat4("view", view);
        flagShader.setMat4("projection", projection);
        flagShader.setFloat("time", timeValue);
        flagShader.setFloat("colCount", (GLfloat)colCount);
        flagShader.setFloat("rowCount", (GLfloat)rowCount);
        flagShader.setMat4("model", model);
        glUseProgram(flagShader.ID);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, colCount * rowCount);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
TextureStruct loadTexture(char const * path)
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

    return TextureStruct(textureID, width, height);
}