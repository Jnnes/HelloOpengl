#ifndef  SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glew/glew.h>
#include <custom/log.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Shader
{
    const char *TAG = "SHADER";
public:
    // 程序ID
    GLuint Program;

    // 构造器读取并构造着色器
    Shader(const GLchar * vertexPath, const GLchar* fragmentPath);
    
    void use();
    void setVec3(const char *uniformName, float val1, float val2, float val3);
    void setVec3(const char *uniformName, glm::vec3);
    void setFloat(const char *uniformName, float val);
    void setMat4(const char *uniformName, const float *);
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    //1.从文件路径中获取顶点和片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //保证ifstream可以抛出异常
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);

    try {
        //打开shader文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        //读取文件内容到文件流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //关闭文件
        vShaderFile.close();
        fShaderFile.close();
        //转换流至GLchar数组
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        Log::e<const char *>(TAG, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    //2.编译着色器
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // 创建并编译顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);;
    glCompileShader(vertex);
    //获取并打印编译信息
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        Log::e<const char *>(TAG, "ERROE::SHADER::VERTEX::COMPILATION_FAILED\n",infoLog);
    }
    else {
        Log::i(TAG, "Successfully compile shader, path:",vertexPath);
    }

    //创建并编译片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        Log::e<const char *>(TAG, "ERROE::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog);
    }
    else {
        Log::i(TAG, "Successfully compile shader, path:", fragmentPath);
    }

    //创建着色器程序，attachs着色器并link
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);

    Log::i(TAG, "Link shaderPragram");

    //已经link，删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(void) {
    glUseProgram(this->Program);
}

void Shader::setVec3(const char *uniformName, float val1, float val2, float val3) {
    glUniform3fv(glGetUniformLocation(this->Program, uniformName), 1, glm::value_ptr(glm::vec3(val1, val2, val3)));
}

void Shader::setVec3(const char *uniformName, glm::vec3 val) {
    glUniform3fv(glGetUniformLocation(this->Program, uniformName), 1, glm::value_ptr(val));
}

void Shader::setFloat(const char *uniformName, float val) {
    glUniform1f(glGetUniformLocation(this->Program, uniformName), val);
}

void Shader::setMat4(const char *uniformName, const float * value_ptr) {
    glUniformMatrix4fv(glGetUniformLocation(this->Program, uniformName), 1, GL_FALSE, value_ptr);
}

#endif // ! SHADER_H

