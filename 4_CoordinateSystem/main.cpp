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
const char windowsTitles[] = "CoordinateSystem";

////定义函数原型
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////找到正确的函数并赋值给指针
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");

void key_callback(GLFWwindow*, int, int, int, int);

GLfloat opcity = 0.2f;
GLfloat vertices[] = {
    -1.0f, 0.0f, 0.0f, // 第一个三角形
    -0.5f, 0.5f, 0.0f,
    0.0f, 0.0f, 0.0f,
};
float vertices1[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f,1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(0.0f,  0.0f,  0.0f),
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
    glfwSetKeyCallback(window, key_callback);

    if (glewInit()) {
        Log::e(TAG, "Failed init glew");
    }
    else {
        Log::i(TAG, "Init glew success");
        GLint nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        Log::i<std::string>(std::string(TAG), "max vertex attributes supportes:", std::to_string(nrAttributes));
    }

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
    int widthImg, heightImg;
    unsigned char* image = SOIL_load_image("container.png", &widthImg, &heightImg, 0, SOIL_LOAD_RGB);
    glActiveTexture(GL_TEXTURE0);
    GLuint texture;//声明纹理对象ID
    glGenTextures(1, &texture);//创建一个纹理对象    
    glBindTexture(GL_TEXTURE_2D, texture);//绑定纹理对象，后面对纹理的操作都是在这个纹理ID上
    //从图片数据中生成纹理，放到之前绑定的纹理对象中
    //第二个参数时0表示只生成Mipmap中0级别的纹理，如果需要生成其他级别的修改0即可
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);//使用之前的纹理对象创建Mipmap，生成所有级别的Mipmap
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);//解绑

    int widthImg1, heightImg1, channels;
    unsigned char* image1 = SOIL_load_image("awesomeface.png", &widthImg1, &heightImg1, &channels, SOIL_LOAD_RGB);
    glActiveTexture(GL_TEXTURE1);
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg1, heightImg1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    //glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image1);
    float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 转换纹理的操作完成后再开启4字节对齐

    Shader shader1("shader/shader1.vert", "shader/shader1.frag");

    // 创建模型矩阵，将顶点从模型坐标转换到世界坐标
    // 这里只是在模型空间以x轴为旋转轴，将物体往屏幕内的方向，旋转55度，
    // 因为定义的物体模型空间的原点，与世界坐标的原点都在0,0,0,点，
    // 所以下面的模型矩阵只需要定义模型相对自己的旋转角度就可以了
    // 否则还要定义模型坐标系相对世界坐标系的转换
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // 创建观察矩阵将顶点从世界坐标系转换到是相机坐标系
    // 这里只是模拟摄像机在[0， 0， 3]位置，所以该观察矩阵是将物体往屏幕里平移3个单位
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // 创建透视投影矩阵，将相机坐标系中的顶点转换到裁剪空间（平面，二维）
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);

    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //其他还有深度缓冲，模板缓冲


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //
        // .. 绘制物体
        //glDrawArrays(GL_TRIANGLES, 0, 3);  

        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

        glEnable(GL_DEPTH_TEST);
        
        shader1.use();
        glActiveTexture(GL_TEXTURE0); // 因为上面创建纹理时绑定了一次，但是修改完纹理后马上解绑了，所以这里必须重新绑定
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader1.Program, "ourTexture1"), 0);

        glActiveTexture(GL_TEXTURE1);// 因为上面创建纹理时绑定了一次，但是修改完纹理后马上解绑了，所以这里必须重新绑定
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader1.Program, "ourTexture2"), 1);

        GLfloat timeValue = (GLfloat)glfwGetTime();
        //glUniform1f(glGetUniformLocation(shader1.Program, "angle"), 10 * timeValue); // 输入参数设置旋转角度

        glUniform1f(glGetUniformLocation(shader1.Program, "opcity"), opcity); // 按键控制的透明度
        
        glm::mat4 modeltemp;
        modeltemp = glm::rotate(model, glm::radians(timeValue * 5), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeltemp));
        glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0));
        trans = glm::rotate(trans, glm::radians(3.0f * timeValue), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(1.2, 1.2, 1.2));
        glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "trans"), 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO1);

        int count = 0;
        for (int i = 0; i < 10; i++) {
            glm::mat4 modeltemp;
            modeltemp = glm::translate(modeltemp, cubePositions[i]);
            if (count++ < 3) {                
                GLfloat angle = 20.0f * (i + 1) * timeValue;
                modeltemp = glm::rotate(modeltemp, glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.5f));
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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
}