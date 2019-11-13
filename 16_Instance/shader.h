#ifndef  SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glew/glew.h>
#include "log.h"

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Shader
{
    const char *TAG = "SHADER";
public:
    // ����ID
    GLuint Program;
    // ��������ȡ��������ɫ��
    Shader(const GLchar * vertexPath, const GLchar* fragmentPath);
    void addGeometryShader(const GLchar* geomPath);
    void use();
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
    }
};

void Shader::addGeometryShader(const GLchar* geomPath) {
    std::string geometryCode;
    std::ifstream gShaderFile;
    gShaderFile.exceptions(std::ifstream::badbit);
    try {
        gShaderFile.open(geomPath);
        std::stringstream  gShaderStream;
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        Log::e<const char *>(TAG, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }

    const GLchar* gShaderCode = geometryCode.c_str();

    GLuint geometry;
    GLint success;
    GLchar infoLog[512];

    //���������뼸����ɫ��
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        Log::e<const char *>(TAG, "ERROE::SHADER::GEOMETRY::COMPILATION_FAILED\n", infoLog);
    }
    else {
        Log::i(TAG, "Successfully compile shader, path:", geomPath);
    }

    //������ɫ������attachs��ɫ����link
    glAttachShader(this->Program, geometry);
    glLinkProgram(this->Program);

    Log::i(TAG, "Link shaderPragram");

    //�Ѿ�link��ɾ����ɫ��
    glDeleteShader(geometry);
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    //1.���ļ�·���л�ȡ�����Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //��֤ifstream�����׳��쳣
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);

    try {
        //��shader�ļ�
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        //��ȡ�ļ����ݵ��ļ�����
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //�ر��ļ�
        vShaderFile.close();
        fShaderFile.close();
        //ת������GLchar����
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        Log::e<const char *>(TAG, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    //2.������ɫ��
    GLuint vertex, fragment, geometry;
    GLint success;
    GLchar infoLog[512];

    // ���������붥����ɫ��
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);;
    glCompileShader(vertex);
    //��ȡ����ӡ������Ϣ
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);        
        Log::e<const char *>(TAG, "ERROE::SHADER::VERTEX::COMPILATION_FAILED\n",infoLog);
    }
    else {        
        Log::i(TAG, "Successfully compile shader, path:",vertexPath);        
    }

    //����������Ƭ����ɫ��
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        Log::e<const char *>(TAG, "ERROE::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog);
    }
    else {
        Log::i(TAG, "Successfully compile shader, path:", fragmentPath);
    }
    
    //������ɫ������attachs��ɫ����link
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);

    Log::i(TAG, "Link shaderPragram");

    //�Ѿ�link��ɾ����ɫ��
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(void) {
    glUseProgram(this->Program);
}

#endif // ! SHADER_H
