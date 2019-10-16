﻿#define GLEW_STATIC
#include <glew/glew.h>
#include<GLFW/glfw3.h>
#include<Windows.h>
#include<iostream>
#include <string>
#include"shader.h"
#include"log.h"

static const char TAG[] = "MAIN";

const char windowsTitles[] = "HelloShader";

////定义函数原型
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////找到正确的函数并赋值给指针
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");

void key_callback(GLFWwindow*, int, int, int, int);

GLfloat vertices[] = {
    - 1.0f, 0.0f, 0.0f, // 第一个三角形
    -0.5f, 0.5f, 0.0f,
    0.0f, 0.0f, 0.0f, 
};
GLfloat vertices1[] = {
    // 位置              // 颜色
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f 
};

GLuint indices[] = {
    0,1,2,
};

GLuint indices1[] = {
    0,1,2,
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

    Shader shader("shader/shader.vert", "shader/shader.frag");
    Shader shader1("shader/shader1.vert", "shader/shader1.frag");

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);    
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT); //其他还有深度缓冲，模板缓冲

        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //
        // .. 绘制物体
        //glDrawArrays(GL_TRIANGLES, 0, 3);  
        shader.use(); 
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        glUniform4f(glGetUniformLocation(shader.Program, "ourColor"), 0.0f, greenValue, 0.0f, 1.0f); // 必须先使用shaderpragram
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

        shader1.use();
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
        // 
        glBindVertexArray(0);

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