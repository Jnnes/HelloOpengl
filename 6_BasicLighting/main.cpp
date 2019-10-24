#define GLEW_STATIC
#include <glew/glew.h>
#include<GLFW/glfw3.h>
#include<soil/SOIL.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<Windows.h>
#include<iostream>
#include <string>

#include <custom/shader.h>
#include <custom/log.h>

static const char TAG[] = "MAIN";
int screenWidth = 800;
int screenHeight = 600;
const char windowsTitles[] = "Camera";

////定义函数原型
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////找到正确的函数并赋值给指针
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement(void);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = 400, lastY = 300;

GLfloat yaw;
GLfloat pitch;
GLfloat aspect = 45.0f;
bool firstMouse = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(1.0f, 0.5f, 2.0f);

GLfloat opcity = 0.2f;
GLfloat vertices[] = {
    -1.0f, 0.0f, 0.0f, // 第一个三角形
    -0.5f, 0.5f, 0.0f,
    0.0f, 0.0f, 0.0f,
};
float vertices1[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f),
};

GLuint indices[] = {
    0,1,2,
};

GLuint indices1[] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
};

GLfloat texCoords[] = {
    0.0f, 0.0f, // left bottom
    1.0f, 0.0f, // right bottom
    0.5f, 1.0f  // up center
};

int main(void) {
    Log::i("start ----------");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, windowsTitles, nullptr, nullptr);    
    glfwSetKeyCallback(window, key_callback);
    if (window == nullptr)
    {
        Log::e(TAG, "Faild to create GLFW window");

        glfwTerminate();
        return -1;
    }
    else
        Log::i(TAG, "Create window success");

    glfwMakeContextCurrent(window);

    //从GLFW中获取视口的维度而不设置为800*600
    //是为了让它在高DPI的屏幕上（比如说Apple的视网膜显示屏）也能正常工作。
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    if (glewInit()) {
        Log::e(TAG, "Failed init glew");
    }
    else {
        Log::i(TAG, "Init glew success");
        GLint nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        Log::i<std::string>(std::string(TAG), "max vertex attributes supportes:", std::to_string(nrAttributes));
    }

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 从此刻起，我们使用的任何在GL_ARRAY_BUFFER目标上的缓冲调用都会用来配置当前VBO
    // 然后我们可以调用GLBufferData函数，他会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
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

    Shader shader1("shader/shader1.vert", "shader/shader1.frag");    
    Shader lightShader("shader/light.vert", "shader/light.frag");

    while (!glfwWindowShouldClose(window)) {
        GLfloat timeValue = (GLfloat)glfwGetTime();
        deltaTime = timeValue - lastFrame;
        lastFrame = timeValue;
        do_movement();
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //其他还有深度缓冲，模板缓冲
        
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
        // 创建透视投影矩阵，将相机坐标系中的顶点转换到裁剪空间（平面，二维）
        glm::mat4 projection = glm::perspective(glm::radians(aspect), (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);
        glm::mat4 model;

        // 渲染光源a
        lightPos.x = sin(timeValue);
        lightPos.z = cos(timeValue);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.use();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        shader1.use();        
        glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));               
        glUniform3fv(glGetUniformLocation(shader1.Program, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        glUniform3fv(glGetUniformLocation(shader1.Program, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform3fv(glGetUniformLocation(shader1.Program, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shader1.Program, "viewPos"), 1, glm::value_ptr(cameraPos));
        glBindVertexArray(VAO1);

        int count = 0;
        for (int i = 0; i < 1; i++) {
            glm::mat4 modeltemp;
            modeltemp = glm::translate(modeltemp, cubePositions[i]);
            modeltemp = glm::rotate(modeltemp, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            if (count++ < 3) {                
                GLfloat angle = 20.0f * (i + 1) * 1; //timeValue
                //modeltemp = glm::rotate(modeltemp, glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.5f));
            }           

            glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "model"), 1, GL_FALSE,glm::value_ptr(modeltemp));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        }
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);        
    }

    glfwTerminate();
    return 0;
    
}

bool keys[1024];
void do_movement(void) {
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_UP]) {
        glm::vec3 cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
        cameraFront += cameraSpeed * cameraUp;
        cameraUp = glm::normalize(glm::cross(cameraRight, (cameraFront - cameraPos)));
    }        
    if (keys[GLFW_KEY_DOWN]) {
        glm::vec3 cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
        cameraFront -= cameraSpeed * cameraUp;
        cameraUp = glm::normalize(glm::cross(cameraRight, (cameraFront - cameraPos)));
    }
    if (keys[GLFW_KEY_LEFT]) {
        glm::vec3 cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
        cameraFront -= cameraSpeed * cameraRight;
        cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
    }
    if (keys[GLFW_KEY_RIGHT]) {
        glm::vec3 cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
        cameraFront += cameraSpeed * cameraRight;
        cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
    }

        
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;

    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        opcity += 0.03f;
        if (opcity > 1.005f)
            opcity = 1.0f;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        opcity -= 0.03f;
        if (opcity < 0.005f)
            opcity = 0.0f;
    }
    else if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (aspect >= 1.0f && aspect <= 45.0f)
        aspect -= yoffset;
    if (aspect <= 1.0f)
        aspect = 1.0f;
    if (aspect >= 45.0f)
        aspect = 45.0f;
}