#define GLEW_STATIC
#include <glew/glew.h>
#include<GLFW/glfw3.h>
#include<Windows.h>
#include<iostream>
#include <string>
#include "shader.h"
#include"log.h"

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

static const char TAG[] = "MAIN";

const char windowsTitles[] = "16_Instance";

////定义函数原型
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////找到正确的函数并赋值给指针
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");

void key_callback(GLFWwindow*, int, int, int, int);

float quadVertices[] = {
    // 位置          // 颜色
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};

int main(void) {
    Log::i("start ----------");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, windowsTitles, nullptr, nullptr);
    if (window == nullptr)
    {
        Log::e(TAG, "Faild to create GLFW window");
        
        glfwTerminate();
        return -1;
    }else
        Log::i(TAG, "Create window success");

    glfwMakeContextCurrent(window);
    
    //从GLFW中获取视口的维度而不设置为800*600
    //是为了让它在高DPI的屏幕上（比如说Apple的视网膜显示屏）也能正常工作。
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(window, key_callback);

    if (glewInit()){        
        Log::e(TAG, "Failed init glew");
    }        
    else{        
        Log::i(TAG, "Init glew success");
        GLint nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        Log::i<std::string>(std::string(TAG), "max vertex attributes supportes:",std::to_string(nrAttributes));
    } 

    // shader
    Shader shader("Shader/instance.vert", "Shader/instance.frag");

    // 准备数据
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2) {
        for (int x = -10; x < 10; x += 2) {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    /*shader.use();
    for (int i = 0; i < 100; i++) {
        std::stringstream strstr;
        strstr << i;
        std::string tempStr;
        strstr >> tempStr;

        shader.setVec2("translate[" + tempStr + "]", translations[i]);
    }*/

    // vao
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // 告诉了OpenGL该什么时候更新顶点属性的内容至新一组数据
    glBindVertexArray(0);    

    int fps = 0;
    int time = 0;
    int lastTime = 0;
    int fry = 1; // 几秒统计一次
    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();        
        if (time - lastTime >= fry) {
            lastTime = time;
            std::cout << "FPS = " + std::to_string(fps / fry) << std::endl;
            fps = 0;
        }
        fps++;
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT); //其他还有深度缓冲，模板缓冲
        
        // draw
        shader.use();
        glBindVertexArray(VAO);
        //for (int i = 0; i < 100; i++) { // 使用标准方法绘制
        //    shader.setVec2("translate", translations[i]);
        //    glDrawArrays(GL_TRIANGLES, 0, 6);            
        //}
        
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}