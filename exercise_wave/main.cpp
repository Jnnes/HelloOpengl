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

#include <stdlib.h>

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

struct Wave {
    GLfloat clickTime; // �����ʱ��
    glm::vec2 clickPos; // �����λ��
    bool isDestory; // ����ʱ�䱻����
    Wave(GLfloat clickTime, glm::vec2 clickPos) {
        this->clickTime = clickTime;
        this->clickPos = clickPos;
        this->isDestory = false;
    }
};

int durationTime = 15;
int maxClickNum = 200;

vector<Wave> waves;
glm::vec2 curPos;
const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 720;
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool isClickPressed = false;
GLfloat lastTime = -1;

////���庯��ԭ��
//typedef void(*GL_GENBUFFERS) (GLsizei, GLuint*);
//
////�ҵ���ȷ�ĺ�������ֵ��ָ��
//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
TextureStruct loadTexture(char const * path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


GLfloat vertices[] = {
    -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // ����
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f,// ����
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // ����
    1.0f, -1.0f, 0.0f, 1.0f, 1.0f // ����
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
        
    Shader flagShader("wave.vert", "wave.frag");

    TextureStruct textureStruct = loadTexture("wave.jpg");
    GLuint texture = textureStruct.id;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int colCount = 800; // X�� ���óɼ���
    int rowCount = 500;   // Y�� ���óɼ���

    // ��֤��Ⱦ������ͼƬ���ݱ���ԭͼƬһ��
    float XDet = 1.0 * textureStruct.width/ textureStruct.height / colCount;
    float YDet = 1.0 / rowCount;

    // ����MVP����
    glm::mat4 model;

    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    //view = glm::rotate(view, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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
    // �Ӵ˿�������ʹ�õ��κ���GL_ARRAY_BUFFERĿ���ϵĻ�����ö����������õ�ǰVBO
    // Ȼ�����ǿ��Ե���GLBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
     
    flagShader.setInt("ourTexture", 0);

    GLfloat lastTime = -1.0;
    int count = 0;
    int det = 2;
    while (!glfwWindowShouldClose(window)) {
        GLfloat timeValue = (GLfloat)glfwGetTime();
        if (timeValue - lastTime > det) {  
            // system("cls"); // ��ִ����������ֹ������shader�Ĵ�����־
            std::cout << "fps = " << to_string(count / det) << std::endl;
            count = 0;
            lastTime = timeValue;
        }
        count++; 

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ����Ĭ����ɫ
        glClear(GL_COLOR_BUFFER_BIT); //����������Ȼ��壬ģ�建��       

        // ��鲨�Ƿ����
        int start = 0, end = waves.size() - 1;
        for (; start <= end; start++) {
            if (timeValue - waves[end].clickTime > durationTime) {
                end--;
                start--;
                continue;
            }

            if (timeValue - waves[start].clickTime > durationTime) {
                // ������Χ
                auto time = waves[start].clickTime;
                auto pos = waves[start].clickPos;
                waves[start].clickPos = waves[end].clickPos;
                waves[start].clickTime = waves[end].clickTime;
                waves[end].clickTime = time;
                waves[end].clickPos = pos;
                end--;
            }
        }

        if (end >= 0) {
            int a = 1;
            a = 2;
        }

        glUseProgram(flagShader.ID);
        flagShader.setFloat("curtime", timeValue);
        flagShader.setInt("clickCount", end + 1);

        for (int i = 0; i <= end; i++) {
            std::stringstream strstr;
            strstr << "click[" << i << "]";
            std::string tempStr;
            strstr >> tempStr;
            flagShader.setVec2(tempStr, waves[i].clickPos);

            std::stringstream ss;
            tempStr = "";
            ss << "clicktime[" << i << "]";
            ss >> tempStr;
            flagShader.setFloat(tempStr, waves[i].clickTime);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)0);
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) { 
    if (action == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            isClickPressed = true;
            {
                auto timeValue = (GLfloat)glfwGetTime();
                if (timeValue - lastTime < 0.05) {
                    return;
                }

                lastTime = timeValue;
                for (size_t i = 0; i < waves.size(); i++)
                {
                    if (timeValue - waves[i].clickTime > durationTime) {
                        waves[i].clickPos = curPos;
                        waves[i].clickTime = timeValue;
                        return;
                    }
                }

                if (waves.size() >= maxClickNum)
                    return;
                auto wave = Wave(timeValue, curPos);
                waves.push_back(wave);
            }
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        default:
            break;
        }
    }        
    else if (action == GLFW_RELEASE)
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                isClickPressed = false;            
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                break;
            default:
                break;
        }
    return; 
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    curPos[0] = (xpos / SCR_WIDTH) * 2.0 - 1.0;
    curPos[1] = ((SCR_HEIGHT - ypos) / SCR_HEIGHT) * 2.0 - 1.0;
    
    if (isClickPressed) {
        auto timeValue = (GLfloat)glfwGetTime();
        if (timeValue - lastTime < 0.02) {
            return;
        }

        lastTime = timeValue;
        for (size_t i = 0; i < waves.size(); i++)
        {
            if (timeValue - waves[i].clickTime > durationTime) {
                waves[i].clickPos = curPos;
                waves[i].clickTime = timeValue;
                return;
            }
        }

        if (waves.size() >= maxClickNum)
            return;
        auto wave = Wave(timeValue, curPos);
        waves.push_back(wave);
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