#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <custom/shader.h>
#include <custom/camer.h>
#include <custom/Model.h>
#include <custom/Light.h>

#include <iostream>
#include <direct.h>
#include <stdio.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
std::string getResPath(std::string);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

vector<float> dirLightData = {
                 // r,x     g,y     b,z
 /* amibent */      0.3f,   0.3f,   0.3f,
 /* diffuse */      1.0f,   1.0f,   1.0f,
 /* specular */     1.0f,   1.0f,   1.0f,
 /* direction */    -1.0f,  0.0f,   -1.0f,
};

vector<vector<float>> pointLightsData = {
    vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.05f,  0.05f,  0.05f,
     /* diffuse */      0.8f,   0.8f,   0.8f,
     /* specular */     1.0f,   1.0f,   1.0f,
     /* position */     0.7f,   0.2f,   2.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
    },
        vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.3f,   0.3f,   0.3f,
     /* diffuse */      1.0f,   1.0f,   1.0f,
     /* specular */     1.0f,   1.0f,   1.0f,
     /* position */     2.3f,   -3.3f,  -4.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
    },
        vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.3f,   0.3f,   0.3f,
     /* diffuse */      1.0f,   1.0f,   1.0f,
     /* specular */     1.0f,   1.0f,   1.0f,
     /* position */     -4.0f,  2.0f,   -12.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
    },
        vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.3f,   0.3f,   0.3f,
     /* diffuse */      1.0f,   1.0f,   1.0f,
     /* specular */     1.0f,   1.0f,   1.0f,
     /* position */     0.0f,   0.0f,   -3.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
    },
};

vector<vector<float>> spotLightsData = {
    vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.0f,   0.0f,  0.0f,
     /* diffuse */      0.8f,   0.0f,   0.0f,
     /* specular */     1.0f,   0.0f,   0.0f,
     /* position */     -1.0f,   0.0f,   0.0f,
                        1.0f,   0.09f,  0.032f, // constant linear quadratic
     /* direction */    1.0f,   0.0f,   0.0f,
                        22.5f,  25.0f,          // cutOff outCutOff
    },
        vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.0f,   0.0f,   0.0f,
     /* diffuse */      0.0f,   0.8f,   0.0f,
     /* specular */     0.0f,   1.0f,   0.0f,
     /* position */     1.0f,   0.0f,   0.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
     /* direction */    -1.0f,  0.0f,   0.0f,
                        22.5f,  25.0f,          // cutOff outCutOff
    },
        vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.0f,   0.0f,   0.0f,
     /* diffuse */      0.0f,   0.0f,   0.8f,
     /* specular */     0.0f,   0.0f,   1.0f,
     /* position */     0.0f,   0.0f,   1.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
     /* direction */    0.0f,   0.0f,   -1.0f,
                        22.5f,  25.0f,          // cutOff outCutOff
    },
        vector<float> {
                     // r,x     g,y     b,z     
     /* amibent */      0.0f,   0.0f,   0.0f,
     /* diffuse */      1.0f,   1.0f,   1.0f,
     /* specular */     1.0f,   1.0f,   1.0f,
     /* position */     0.0f,   0.0f,   -1.0f,
                        1.0f,   0.09f,  0.032f,// constant linear quadratic
     /* direction */    0.0f,   0.0f,   1.0f,
                        22.5f,  25.0f,          // cutOff outCutOff
    },
};

DirLight dirLight;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

void initLightData() {
    dirLight = DirLight(dirLightData);

    for (unsigned int i = 0; i < pointLightsData.size(); i++) {
        pointLights.push_back(PointLight(pointLightsData[i]));
    }

    for (unsigned int i = 0; i < spotLightsData.size(); i++) {
        spotLights.push_back(SpotLight(spotLightsData[i]));
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("shader/model.vert", "shader/model.frag");

    // load models
    // -----------
    Model ourModel(getResPath("/objects/nanosuit/nanosuit.obj"));

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    initLightData();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        // 定向光源
        ourShader.setInt("dirLightNum", 1);
        ourShader.setVec3("dirLight.direction", dirLight.direction);
        ourShader.setVec3("dirLight.ambient", dirLight.ambient);
        ourShader.setVec3("dirLight.diffuse", dirLight.diffuse); // 将光照调暗了一些以搭配场景
        ourShader.setVec3("dirLight.specular", dirLight.specular);

        // 点光源
        ourShader.setInt("pointLightNum", pointLights.size());
        std::string uniformNmae = "pointLights[0].";
        for (int i = 0; i < pointLights.size(); i++) {
            uniformNmae[12] = '0' + i;
            std::string tempStr(uniformNmae);
            ourShader.setVec3((tempStr + std::string("position")).c_str(), pointLights[i].position);

            ourShader.setFloat((tempStr + std::string("constant")).c_str(), pointLights[i].constant);
            ourShader.setFloat((tempStr + std::string("linear")).c_str(), pointLights[i].linear);
            ourShader.setFloat((tempStr + std::string("quadratic")).c_str(), pointLights[i].quadratic);
            ourShader.setVec3((tempStr + std::string("ambient")).c_str(), pointLights[i].ambient);
            ourShader.setVec3((tempStr + std::string("diffuse")).c_str(), pointLights[i].diffuse);
            ourShader.setVec3((tempStr + std::string("specular")).c_str(), pointLights[i].specular);
        }

        // 聚光
        ourShader.setInt("spotLightNum", spotLights.size());
        uniformNmae ="spotLights[0].";
        for (unsigned int i = 0; i < spotLights.size(); i++) {
            uniformNmae[11] = '0' + i;
            std::string tempStr(uniformNmae);
           
            ourShader.setVec3((tempStr + std::string("direction")).c_str(), spotLights[i].direction);
            ourShader.setVec3((tempStr + std::string("position")).c_str(), spotLights[i].position);
            ourShader.setFloat((tempStr + std::string("cutOff")).c_str(), glm::cos(glm::radians(spotLights[i].cutOff)));
            ourShader.setFloat((tempStr + std::string("outCutOff")).c_str(), glm::cos(glm::radians(spotLights[i].outCutOff)));
            ourShader.setVec3((tempStr + std::string("ambient")).c_str(), spotLights[i].ambient);
            ourShader.setVec3((tempStr + std::string("diffuse")).c_str(), spotLights[i].diffuse); // 将光照调暗了一些以搭配场景
            ourShader.setVec3((tempStr + std::string("specular")).c_str(), spotLights[i].specular);
            ourShader.setFloat((tempStr + std::string("constant")).c_str(), spotLights[i].constant);
            ourShader.setFloat((tempStr + std::string("linear")).c_str(), spotLights[i].linear);
            ourShader.setFloat((tempStr + std::string("quadratic")).c_str(), spotLights[i].quadratic);
        }        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
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