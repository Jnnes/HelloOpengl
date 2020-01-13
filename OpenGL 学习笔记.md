#OpenGL 学习笔记
修改记录：
1. 2020.01.13 调整错别字


#OpenGL渲染管线之概述 （一）
##前言：
&emsp;&emsp;本系列学习可能不适合对OpenGL没有任何了解的学习者。因为本系列只是作者基于自己的学习历程及加上自己对某些功能的设计思路思考得到，并且完成度不是很高。适合于对OpenGL已经有了一定了解，至少需要知道渲染管线主要有哪些流程以及图形渲染过程需要经过哪些变换。

##概述：

&emsp;&emsp;opengl渲染管线是为了独立出渲染图形的每个过程，因为基于目前的渲染图形理论，渲染同一个场景中的大多数物体的计算过程是相同的，甚至不同场景里的大多数物体的计算过程也可以是相同的，那么就需要将这些相同的操作独立出来，避免重复造轮子。

##流水线阶段：

&emsp;&emsp;***模型数据：***该阶段是定义绘制的图形的位置，大小，颜色等。例如：我们需要在世界坐标系的原点处画一个平面三角形，那么我们至少需要定义这个三角形的三个顶点分别在什么位置（3D空间）。如果需要，我们还可以定义三个顶点分别是什么颜色。基于这些信息，我们就可以确定这个三角形在什么位置和大小。
&emsp;&emsp;***顶点着色器：***该阶段是转换模型数据里的3D坐标到裁剪坐标系。因为我们的模型数据的坐标都是局部坐标，可物体要真实地被渲染在屏幕上，还需要经过**局部坐标系-世界坐标系-观察坐标系-投影坐标系-屏幕坐标系**等过程，当然也可以直接从局部坐标系到屏幕坐标系，只是这样的话，这个空间里的所有物体都需要重复实现 **世界坐标系-观察坐标系-投影坐标系-屏幕坐标系**这个计算过程，而这个过程在这个空间里的大部分物体都是完全一样的。基于此，我们需要一个顶点着色器来应用不同物体之间的不同的3D变化，最大程度地增加重用减小耦合。
&emsp;&emsp;***图元装配：***该阶段是将模型的定义具体化。因为上面两个阶段的物体只有最小定义，例如三角形由三个顶点即可确定。但是在屏幕上显示的却是完整的三角形（点与点之间看起来是连续的线段）。更加关键的是，我们的屏幕是离散的像素点，从三角形的三个顶点，到屏幕上离散的点的集合组成的三角形（只是我们从屏幕上看起来认为是三角形，因为三角形是连续图形的概念），我们不可避免地需要从三角形的三个顶点推断出三角形的连续图形，再将连续图形离散化，显示在屏幕上。而图元装配阶段就是从三个顶点推断出三角形连续图形的定义的过程。
&emsp;&emsp;***几何着色器：***该阶段是在上一阶段的基础上，修改上一阶段的图元，用以实现特定功能。
&emsp;&emsp;***光栅化：***该阶段将图元映射为最终屏幕上响应的像素，也即离散化过程。
&emsp;&emsp;***片段着色器：***该阶段计算每一个像素的最终颜色。所有OpenGL高其效果产生于此。通过使用3D场景的光照、阴影、光的颜色等数据来计算最终像素颜色。
&emsp;&emsp;***测试与混合：***该阶段使用各种测试和Blend；测试用来判断像素是否应该被抛弃，混合用于混合多个像素的颜色。
&emsp;&emsp;大牛们对流水线的总结理解：

[OpenGL渲染管线---概述（一）][1] 详细讲述了OpenGL中各个组成模块在流水线中发挥的作用，及对比固定管线与可编程管线，加固理解。

固定管线，图片引用自上文。下图中省略了图元装配
![此处输入图片的描述][2]

详细可编程管线图示，引用上文：
![此处输入图片的描述][3]
  
#OpenGL渲染管线之坐标系 （二）
##概述：
&emsp;&emsp;在OpenGL渲染的整个流程中，共涉及到这几个坐标系：局部坐标系（Object or model coordinates）、世界坐标系（World coordinates）、摄像机坐标系（Eye、Camera coordinates）、裁剪坐标系（Clip coordinates）、屏幕坐标系（Screen Coordinates）。
![此处输入图片的描述][4]

***我们需要用到这几个矩阵：***
&emsp;&emsp;模型矩阵（Model Matrix）、观察矩阵（View Matrix）、投影矩阵（Projection Matrix）。
***坐标系与变换矩阵的关系：***
&emsp;&emsp;局部坐标系 ——（模型矩阵）—— 世界坐标系 ——（观察矩阵）——摄像机坐标系——（投影矩阵）——裁剪坐标系
&emsp;&emsp;各个矩阵及坐标系概述：

***局部坐标系：***局部坐标系是物体内部的坐标系，他描述的是这个物体内部各个组成部分之间的相互关系，每个物体都会有一个他自己的局部坐标系。例如：一个正方形我们可以使用四个顶点来定义，我们约定，该正方形的局部坐标系的原点在正方向中心，那么正方形的四个顶点分别为[0.5, 0.5]，[-0.5, 0.5]，[-0.5, -0.5]，[0.5, -0.5]（逆时针方向）。如果有第二个相同的但是在不同位置的正方形，那么四个顶点的局部坐标也完全相同。不同的是这连个正方形世界坐标不同。

***模型矩阵：***将物体的坐标从局部坐标系转换到世界坐标系。上面的两个正方形的四个顶点的局部坐标完全一样，但是模型矩阵不一样，导致在世界坐标系中的位置不一样。如果Cube0在原点，那么Cube0的模型矩阵为E，Cube1的中心在X轴的1.0位置，那么就相当于将Cube0往X轴方向平移1.0距离，那个这个平移变换就是Cube1的模型矩阵，同理旋转缩放。

***世界坐标系：***描述所有物体的顶点之间的相对位置。

***观察矩阵：***将世界中的物体坐标转换到摄像机坐标系，用以模拟摄像机在世界的不同位置、以不同角度、不同视角观察世界。这里需要注意的是，摄像机坐标系的Z轴正方向对应的是摄像机屁股对应的方向，而不是镜头对应的方向。这样定义的好处是：Opengl屏幕坐标系的Z轴向屏幕外，当摄像器的Z轴和屏幕坐标系的Z轴方向一样时便于计算，否则的话，转换过程都需要加上Z轴翻转，但是摄像机又需要向着屏幕内的物体，这样就导致了摄像机镜头向着屏幕里（Z轴负方向），摄像机屁股向着屏幕外（Z轴正方向）。

***摄像机坐标系：***以摄像机为原点，描述世界中所有物体的相对位置。

***投影矩阵：***将摄像机坐标系中的物体投影到裁剪空间，也即将3D投影到2D的过程。

***裁剪坐标系：***描述最终显示到屏幕上的物体的相对位置，但不是距离。该坐标系的XY轴与屏幕坐标系的XY轴在同一个平面上，不同的地方在于，当我们在OpenGL中设置了ViewPort后，ViewPort空间即可认为是屏幕空间，它的原点在ViewPort的左下角。这时我们需要经过平移、旋转、缩放等转换（Z轴不变）得到屏幕坐标系内的顶点坐标。
## 两种投影方式：
***正射投影：***平行投影，物体在投影面上的相对距离不会因为与投影面的距离的远近而改变。
![此处输入图片的描述][5]

***透视投影：***射线投影，距离越远的线段，在投影面上的距离越短。
![此处输入图片的描述][6]

#OpenGL渲染管线之纹理（三）
##概述：

&emsp;&emsp;纹理是一个2D图片（也可以是1D或者3D），用来让图形看起来更真实。是一种很简单就能够模拟真实的方法，因为如果不使用纹理，那么我们就必须为所有细节构建顶点和图形，这样的开销会非常大，有了纹理后，我们就只需要少很多的顶点，然后把纹理贴上去就可以了，即使有些不真实，但是很难看出来。

##纹理坐标：

&emsp;&emsp;纹理坐标的原点在左下角；S、T、R 对应 X、Y、Z但是在存储图片的时候数据是从左上角开始存，这样导致读取的坐标Y轴被反转。OpenGL默认开启4字节对齐，如果读取的数据不是4字节对齐的，将图片数据转换到纹理对象会导致图像倾斜，因为数据使用一维数组存储，同时使用width和height查询像素的具体位置。可以使用glPixelStorei(GL_UNPACK_ALIGNMENT, 1)设置字节对齐方式。

##纹理环绕方式：
- GL_REPEAT：纹理的默认行为，重复纹理图形
- GL_MIRRORED_REPEAT：镜像重复
- GL_CLAMP_TO_EDGE：约束到边缘
- GL_CLAMP_BORDER：约束到指定边框，可以通过 glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor)设置边框颜色

eg： glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//设置X轴方向为镜像重复

##纹理过滤：
&emsp;&emsp;纹理过滤指的是在采样阶段，当需要采样的坐标不存在时，采用怎样的策略使用待采样坐标附近的颜色来推算待采样坐标的颜色（很显然，绝大部分情况下都是无法直接命中的）。

- GL_NEAREST：最近邻过滤，看起来锐利，使用靠近目标坐标的的纹理坐标处的颜色。建议用在当Minify（缩小）的情况下。
- GL_LINEAR：线性过滤，看起来模糊，使用目标坐标附近的纹理颜色线性插值。建议用在Magnify（放大）的情况下。
为了进一步让纹理再使用时看起来更真实，使用了多级渐远纹理（Mipmap）。Mipmap是一套纹理，他们从第0个纹理（原始纹理）开始，后面一个纹理的边长都为前一个的二分之一，用以模拟纹理在所有缩放尺寸的纹理。由于使用二分之一作为倍数，所以还是会出现不真实的边界，所以也可以对Mipmap使用多种纹理过滤方式，例如：
1. GL_NEAREST_MIPMAP_NEAREST：使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
2. GL_LINEAR_MIPMAP_NEAREST：使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
3. GL_NEAREST_MIPMAP_LINEAR：在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
4. GL_LINEAR_MIPMAP_LINEAR：在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样。
纹理单元：一个纹理的位置通常称为一个纹理单元。我们使用glActiveTexture激活指定纹理，后面的对纹理的操作都是在该激活的纹理上。


#OpenGL渲染管线之着色器（四）
本文所介绍的着色器基于OpenGL 3.3

##前言：
&emsp;&emsp;OpenGL实现可编程逻辑管线，开发人员就可以自定义其中的某些流程，从 《penGL渲染管线之概述 （一）》（[点击进入][7]）可以看出，可编程逻辑管线的实现方式只是将之前的固定管线中可编程的部分提取出来供开发人员自定义。目前，开发人员可以使用的包括顶点着色器、几何着色器、片元着色器。它们的功能请继续阅读《penGL渲染管线之概述 （一）》。

##概述：
&emsp;&emsp;着色器是OpenGL可编程逻辑管线中的某些流程，目前可供开发人员自定的包括顶底、几何、片元着色器。使用GLSL（OpenGL Shader Language）编程，因为它对向量和矩阵有特别的支持，所以很适合用于图形计算。

##语法：

&emsp;&emsp;我们使用 in 来修饰需要传入当前着色器的变量，使用out修饰当前着色器需要往后传递的变量，使用uniform修饰所有着色器公用变量。

```
//使用OpenGL3.3；并且在core模式下运行，只有这样才能使用可编程管线
#version 330 core 
layout (location = 0) in type in_variable_nam;
layout (location = 1) in type in_variable_name;
out type out_variable_name;
uniform type uniform_name;
int main()
{
    // 处理输入并进行一些图形操作
    ...
    // 输出处理过的结果到输出变量
    out_variable_name = weird_stuff_we_processed;
}
```

&emsp;&emsp;着色器在编译的时候会自动串接多个shader间的in out变量。对于Uniform变量，我们需要通过手动传入或者OpenGL自动传入。有下面几种数据类型:int, float, double, uint, bool。同时我们还可以使用vec和mat，例如vec4，mat4，mat3x4等，我们可以在前面加b, i,u,d分别表示内部的数据类型为bool, int, uint, double。默认情况下是float。后面带数字则表示向量或矩阵大小。

&emsp;&emsp;我们可以使用xyzw、rgbd、stpq来分别获取向量的第1/2/3/4个分量，并且可以任意组合，例如：vec2 someVec = lastVec.zx。也可以使用它们的任意组合来创建一个和原来一样长的同类型新变量，例如：vec4 someVec = vec4(lastVec.zx, lastVec.x, lastVec.y)。

&emsp;&emsp;对于输入，我们使用layout (location = 0)标识符来限定，具体用法在下个文章里详细介绍。location 后面的值表示第几个属性，内容需要使用glVertexAttribPointer() 和glEnableVertexAttribArray()定义，具体的在下个文章介绍。

&emsp;&emsp;对于Uniform，我们先获取着色器程序对象中uniform变量的位置，然后手动改变该变量的值（并不是所有的Uniform变量都通过下面的方式更改）例如：
```
GLfloat timeValue = glfwGetTime();
GLfloat greenValue = (sin(timeValue) / 2) + 0.5;

//通过变量名称，获取Uniform变量的位置
GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

// 必须要使用着色器程序才能够改变里面的变量值
glUseProgram(shaderProgram); 

// 通过uniform变量的的位置改变变量值
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```


#OpenGL渲染管线之简单示例（五）

##前言：
&emsp;&emsp;这一章我们将对前面的所有知识进行实践，通过OpenGL编程，完成一个简单的OpenGL程序。你可以通过访问我的GitHub获取我学习的HelloOpenGL项目。这个项目需要的所有头文件及库均配置在项目中，下载后无需再额外配置环境，但由于作者并不想重复配置多个平台，请在Debug - Win32（x86）环境下编译。另外，作者把很多理解都写在了注释中！！！

项目地址：https://github.com/Jnnes/HelloOpengl

引用网址：https://learnopengl-cn.readthedocs.io/zh/latest/ 如遇到问题，请上该网址查找。

&emsp;&emsp;OpenGL编程前需知：在开始OpenGL编程之前，我们需要提前了解一些东西，包括以下几方面
OpenGL是一个大的状态机，我们可以把渲染的每个阶段都看成一个状态，而每个状态的动作由各种属性控制，当前状态的动作执行完后会自动进入下一个状态，直到完成渲染，我们需要使用属性设置函数来设置属性，通过属性应用函数来应用属性。
&emsp;&emsp;在状态机中，我们把很多东西都当做对象来管理，这样当状态机中有多个相同类型的对象时，我们可以通过绑定某种类型对象和他实际存储数据发挥作用的对象，那么当状态机需要使用该类型时，则会自动去使用绑定该类型的对象。例如：纹理对象，缓冲对象等。所有的对象都继承自Object，对于这种使用对象来管理的数据，我们在使用之前都需要创建一个对象，然后将该类型对象绑定到新创建的这个对象实例上。

```
// OpenGL的状态
struct OpenGL_Context 
{
    ...
    object* object_Window_Target;
    ...     
};

// 创建对象，后面都通过该对象id找到OpenGL中的这个对象
GLuint objectId = 0;
glGenObject(1, &objectId);

// 绑定对象至上下文，指定GL_WINDOW_TARGET 类型的对象 存储在 objectId
glBindObject(GL_WINDOW_TARGET, objectId);

// 设置GL_WINDOW_TARGET对象的一些选项，也即对 objectId的对象进行操作 
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);

// 将上下文的GL_WINDOW_TARGET对象设回默认，解除GL_WINDOW_TARGET 与objectId的绑定
// 后面对GL_WINDOW_TARGET的操作将不会操作objectId
glBindObject(GL_WINDOW_TARGET, 0);
```

但是，有些特殊对象并不是使用上面的函数，例如纹理：

```
// 创建一个纹理对象，并保存其唯一对象id，OpenGL内所有的对象Id均不相同，不论什么类型。
GLuint texture;
glGenTextures(1, &texture);

// 绑定GL_TEXTURE_2D 的 对象ID为 texture
glBindTexture(GL_TEXTURE_2D, texture);

// 往GL_TEXTURE_2D 中写数据，因为绑定的GL_TEXTURE_2D类型的对象的ID是texture，
// 那么下面的数据会写入到ID为texture的对象上
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

// 绑定GL_TEXTURE_2D 的 对象ID为 0，这段后面的代码如果操作了GL_TEXTURE_2D，将是在id为0
// 的纹理对象上，而id为零的纹理对象是不存在的，所以这句代码实际意思是解绑定GL_TEXTURE_2D 对象ID
glBindTexture(GL_TEXTURE_2D, 0);
```

&emsp;&emsp;其他还有VAO、VBO、VFO等等，虽然创建、绑定、修改这种对象时使用的函数名各不相同，但是他们都有一个统一的流程，这个是OpenGL设计理念决定的。他们都遵循创建，绑定，修改三步走，除非主动修改绑定对象，否则OpenGL上下文中的该类型对象绑定的ID始终不变。

##正式编程：
下面代码省略include头文件，详细请看项目源代码。

###1. 创建窗口

&emsp;&emsp;创建窗口的详细过程可以参照LearnOpengl入门的创建窗口（[点击进入][8]），或者使用文章开头提供的项目。下面只对代码进行解释。

&emsp;&emsp;因为OpenGL只是一个标准，只提供接口定义，内部实现由各大硬件厂商完成，并且还可以跨平台，所以我们需要一个库，它能够帮我们找到不同平台上的OpenGL函数到底在硬盘的哪里，glew库实现了这个功能。

&emsp;&emsp;另外我们可以使用glfw库方便我们创建窗口。

```
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
    // 初始化glfw
    glfwInit(); 

    // 设置OpenGL大版本号为3，小版本号为3，也即3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    // 设置OpenGL使用Core模式，可使用可编程管线
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 禁用可变窗口大小
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // 创建一个窗口对象，宽度800像素，高度600像素，标题为"LearnOpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {    
        // 退出程序前，一定要关闭glfw
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, key_callback); // 设置按键回调
    glfwSetScrollCallback(window, scroll_callback); // 设置鼠标移动回调
    glfwSetCursorPosCallback(window, mouse_callback); // 设置鼠标滚轮回调

    // 设置窗口的显示上下文，也即下面OpenGL绘制的内容将存储在window这个窗口对象上
    glfwMakeContextCurrent(window);
    
    // 从GLFW中获取视口的维度而不设置为800*600
    // 是为了让它在高DPI的屏幕上（比如说Apple的视网膜显示屏）也能正常工作。
    // 因为高DPI的屏幕上显示图片上的一个像素点可能需要几个屏幕发光单元
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);    
    
    
    // 初始化glew
    if (glewInit()) {
        Log::e(TAG, "Failed init glew");
        glfwTerminate();
        return -1;
    }
    else {
        Log::i(TAG, "Init glew success");
        
        // 获取当前OpenGL可支持最大纹理数目
        GLint nrAttributes;

        // 我们可以通过glGetxxx来获取OpenGL上下文中的属性，
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        Log::i<std::string>(std::string(TAG), "max vertex attributes supportes:", std::to_string(nrAttributes));
    }
    
    // 我们需要一个主循环来连续绘制并刷新屏幕
    while(!glfwWindowShouldClose(window))
    {
        // 检查事件，包括按键、鼠标移动等等
        glfwPollEvents();

        // 因为数据是一个个像素绘制的，为了使一个屏幕的像素一起显示出来，OpenGL拥有双缓冲机制
        // 当某个缓冲绘制换成后，将其换到前台进行显示，另一个缓冲继续在后台绘制
        glfwSwapBuffers(window);
    }

    // 退出程序前，一定要关闭glfw
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {    
    // 如果按下返回键，则将应该关闭窗口标志位置位，退出main中的主循环
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } 
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    ;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ;
}
```

###2. 缓冲对象

在创建着色器之前，我们需要定义顶点数据。

```
GLfloat vertices[] = { // 深度都为0，表现为在XY坐标系下的三个点
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
```

&emsp;&emsp;我们需要使用VBO（顶点缓冲对象）来存储顶点数据，我们还可以使用VAO（顶点数组对象）来绑定，这样当我们切换不同的VBO时只需要切换绑定不同的VAO就可以了，更大的优点是，我们不仅可以把VBO绑定在VAO上，我们还可以把EBO（顶点索引数组）也绑定在VAO上，这样我们只要切换了VAO就同时切换了VBO和EBO。
![此处输入图片的描述][9]

VAO、VBO、EBO之间的关系
我们可以像创建其他对象一样创建VAO，VBO，EBO

```
GLfloat vertices1[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 0.0f,   // 右上 // 手动在这里翻转Y轴纹理坐标
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 2.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 2.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // 左上
};    
GLuint indices1[] = {
    1,2,3,
    0,1,3,
};

// 创建并绑定顶点缓冲数组
GLuint VAO1;
glGenVertexArrays(1, &VAO1);
glBindVertexArray(VAO1);
    
// 创建、绑定 顶点索引缓冲对象，并设置数据
GLuint EBO1;
glGenBuffers(1, &EBO1);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    
// 创建、绑定 顶点缓冲对象，并设置数据
GLuint VBO1;
glGenBuffers(1, &VBO1);
// 从此刻起，我们使用的任何在GL_ARRAY_BUFFER目标上的缓冲调用都会用来配置当前VBO
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
// 然后我们可以调用GLBufferData函数，他会把之前定义的顶点数据复制到缓冲的内存中
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
glEnableVertexAttribArray(0);
    
// 继续设置第2个顶点属性，对应的是着色器代码里的 layout(location = 1)，和上图中VAO2的第二条黑线
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
    
// 继续设置第2个顶点属性，对应的是着色器代码里的 layout(location = 2) 
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);

// 取消之前顶点缓冲数组的绑定
glBindVertexArray(0);

// 下面绘制的过程最好放在主循环中循环执行，除非你只是想绘制一次就再不刷新
// 我们可以在需要使用VBO 和 EBO时直接使用之前已经绑定好的VAO
glBindVertexArray(VAO1);
// 然后调用glDrawElement，OpenGL就会自动从已绑定的EBO中获取顶点索引，再根据索引在绑定的VBO中获取顶点坐标进行绘制
glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

// 或者我们并不需要顶点索引，我们也可以通过其他方式绘制
// 直接绑定VBO，不使用EBO，然后调用glDrawArrays(), 就会从已绑定的VBO中按顺序获取坐标进行绘制
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
glDrawArrays(GL_TRIANGLES, 0, 3);     
```

&emsp;&emsp;在glBindVertexArray(VAO1)和glBindVertexArray(0)之间的所有对VBO和EBO的操作都会被VAO记录下来，那么当我们再次调用glBindVertexArray(VAO1)时，OpenGL就知道我们需要使用在VAO1中设置好的VBO和EBO。

&emsp;&emsp;在使用glVertexAttribPointer() 时，我们要注意里面的参数，主要是位置偏移。具体的请查看相关手册。

###3.着色器

&emsp;&emsp;要使用着色器，我们必须创建我们自己的着色器对象，然后我们将着色器源码放放入着色器对象，编译它们；我们还需要运行着色器的着色器程序对象，并且将着色器对象放入着色器程序对象中，并链接这个着色器对象，当我们使用某个着色器程序对象时，OpenGL就会自动对待绘制的图形使用之前绑定的着色器对象，它将按照我们再着色器源码中写的那样渲染出图形。
```
//  声明一个GLuint 来存储顶点着色器对象的ID
GLuint vertexShader;

// 创建一个着色器对象，类型是顶点着色器。片元着色器是 GL_FRAGEMENT_SHADER
vertexShader = glCreateShader(GL_VERTEX_SHADER);

// 为顶点着色器对象绑定源码（字符串）
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

// 编译着色器
glCompileShader(vertexShader);

// 输出编译错误信息
GLint success;
GLchar infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}else
    std::cout << "Compile VertexShader success" << std::endl;

/*  
   创建并编译片元着色器 
*/

// 创建着色器程序对象
GLuint shaderProgram;
shaderProgram = glCreateProgram();

// 为着色器程序对象附加着色器对象
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);

// 链接着色器
glLinkProgram(shaderProgram);

// 使用着色器，任何时候，只要我们需要，我们就可以调用这段代码
glUseProgram(shaderProgram);
```
 上面的代码只列出了顶点着色器，片元着色器的创建方法类似。下面的代码是顶点着色器和片元着色器的源码。

```
// shader1.vert 顶点着色器
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 trans;

void main(){
    gl_Position = projection* view*model * vec4(position, 1.0f);
    ourColor = color;
	TexCoord = texCoord;
}
```
```
// 片元着色器
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 fragColor;

// GL_TEXTURE0 的纹理采样器ID，在代码中指定这个uniform为(INT)0即可
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;// GL_TEXTURE1 的纹理采样器ID，同上

uniform float angle;
uniform float opcity;

void main()
{
	float a = angle * 3.1415926 / 180;
	vec2 TexCoord2 = vec2(cos(a) * TexCoord.x - sin(a)*TexCoord.y, TexCoord.x * sin(a) + TexCoord.y * cos(a));
	
    // 混合两个颜色
    fragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord2), opcity);
}
```
 主循环
```
 while (!glfwWindowShouldClose(window)) {
        GLfloat timeValue = (GLfloat)glfwGetTime();
        deltaTime = timeValue - lastFrame;
        lastFrame = timeValue;
        do_movement();
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置默认颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //其他还有深度缓冲，模板缓冲

        glEnable(GL_DEPTH_TEST);
        
        shader1.use();
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);        
    }
```
###4. 使用纹理

&emsp;&emsp;使用纹理我们需要使用某些库来辅助我们读取图片数据，毕竟谁也不想与种类繁多的图片格式打交道。我们可以使用SOIL库或者stb_image等，但他们的作用都只是帮助你更方便使用图片。这里使用的是SOIL。

&emsp;&emsp;很多时候我们需要先禁用4字节对齐，改为1字节对齐，因为OpenGL默认使用4字节对齐，而很多图片他们并没有完整的4通道或者其他原因导致他的数据不是4的整数倍，如果这时OpenGL按照4的倍数来计算图片的尺寸的话往往会存在些偏差，这些偏差表现在显示上就是图片会倾斜。

```
// 禁用4字节对齐
glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

// 使用SOIL库读取图片文件，并存储在image指针指向的区域
int widthImg, heightImg;
unsigned char* image = SOIL_load_image("container.png", &widthImg, &heightImg, 0, SOIL_LOAD_RGB);  

// 启动第1个纹理单元，后面对纹理的操作将在TEXTURE0上进行
// 我们可以重复下面的操作继续往第1,2,3...16个纹理单元上设置数据，
// 这样后面我们只需要切换当前使用的纹理既可以获取之前设置的纹理数据
glActiveTexture(GL_TEXTURE0);

//创建一个纹理对象
GLuint texture;
glGenTextures(1, &texture);

// 并将GL_TEXTURE_2D 绑定的对象指定为该纹理对象,后面对GL_TEXTURE_2D的操作都是在这个纹理ID上
glBindTexture(GL_TEXTURE_2D, texture);

//从图片数据中生成纹理，放到之前绑定的纹理对象中
//第二个参数时0表示只生成Mipmap中0级别的纹理，如果需要生成其他级别的修改0即可
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

// 删除之前从文件读取的图片数据，因为纹理数据已经使用了该图片数据
SOIL_free_image_data(image);

//使用之前的纹理对象创建Mipmap，生成所有级别的Mipmap
glGenerateMipmap(GL_TEXTURE_2D);

// 设置纹理边框
float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

//环绕方式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

// 过滤方式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

glBindTexture(GL_TEXTURE_2D, 0);//解绑

// 转换纹理的操作完成后再开启4字节对齐,也可以在读取。设置纹理数据结束后就执行该操作
glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 

/* 我们可以在主循环中这样使用 */

// 启用着色器程序
shader1.use();   

// 使用第1个纹理单元
// 因为上面创建纹理时绑定了一次，但是修改完纹理后马上解绑了，所以这里必须重新绑定
glActiveTexture(GL_TEXTURE0); 
glBindTexture(GL_TEXTURE_2D, texture);
glUniform1i(glGetUniformLocation(shader1.Program, "ourTexture1"), 0);

// 使用第2个纹理单元
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture1);
glUniform1i(glGetUniformLocation(shader1.Program, "ourTexture2"), 1);
```
###5. 坐标系变换及摄像机移动

&emsp;&emsp;之前的[OpenGL渲染管线之坐标系（二）][10]已经叙述过OpenGL之间的坐标系，上一节的顶点着色器中已经置入了模型矩阵、观察矩阵、投影矩阵。下面我只会记录一些学习过程中的额外内容。
```
// 创建模型矩阵，这里只是将所有模型统一往屏幕内部旋转了timeValue * 5度
glm::mat4 modeltemp;
modeltemp = glm::rotate(model, glm::radians(timeValue * 5), glm::vec3(0.5f, 1.0f, 0.0f));

// 创建观察矩阵，由相机的位置，相机的视线聚焦处，以及相机向上的坐标轴即可确定摄像机坐标系
// 视线方向不与cameraUp垂直时，怎么办？
glm::mat4 view;
view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

// 创建透视投影矩阵，将相机坐标系中的顶点转换到裁剪空间（平面，二维）
glm::mat4 projection;
projection = glm::perspective(glm::radians(aspect), (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);

// 将MVP矩阵置入Shader
glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeltemp));
glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
```
&emsp;&emsp;为了实现可控镜头，我们需要改变镜头的位置，视线方向，以及视角。这些由摄像机当前位置，摄像机视线聚焦位置，以及摄像机向上的坐标轴方向确定。

&emsp;&emsp;只改变摄像机位置和视点位置比较简单，这里不赘述。我们讨论如何改变摄像机角度，我们可以使用欧拉角中的俯仰角pitch、偏航角yaw以及桶滚角roll来描述摄像机当前的方向。这里我们使用另外一种方式，以摄像机向上仰视为例，当我们按下上键，摄像机慢慢向上仰视。

&emsp;&emsp;我们使用视线方向和摄像机头顶方向的轴，获取另一个坐标轴，将视点往Up轴方向移动一定距离，这样我们的Up轴将发生变化，我们再刷新Up轴，即可。注：我们可以对两个向量叉乘得到与这两条向量都垂直的向量，这样可方便根据两条坐标轴获取第三条坐标轴，计算得到的第三条坐标轴的方向遵循右手定则。
```
if (keys[GLFW_KEY_UP]) {
   glm::vec3 cameraRight = glm::normalize(glm::cross((cameraFront - cameraPos), cameraUp));
   cameraFront += cameraSpeed * cameraUp;
   cameraUp = glm::normalize(glm::cross(cameraRight, (cameraFront - cameraPos)));
}
```
另外我们也可以使用鼠标操作来完善整个摄像机变换的过程，文章开头的项目中已经完整实现。

#OpenGL渲染管线之光照(六)
##材质
材质也即物体的光照属性，我们使用材质（Material）来模拟物体在真实光照环境下的光照表现。
```
#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;
```

ambient：环境光照下物体反射光颜色
diffuse：漫反射光照下物体反射光颜色
specular：镜面反射光照下物体反射光颜色
shininess：影响镜面高光的散射/半径
```
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
```
![此处输入图片的描述][11]

三个组成部分的计算方法：
```
// 环境光
vec3 ambient = Light.ambient * Material.ambient;

// 漫反射， lightDir 是片元到光源的方向向量
float diff = max(dot(lightDir, Normal), 0.0);
vec3 diffuse = Light.diffuse * diff * Material.diffuse;

// 镜面反射， viewDir 是片元到摄像机的方向向量
// 计算反射光线时，入射光线是从光源到片元
vec3 reflectDir = reflect(-lightDir, Normal); 

// 幂次越大，坡越抖，也即反射的亮斑越小，镜面程度越高
float spec = pow(max(dot(reflectDir, viewDir),0.0), 32.0);
vec3 specular = Light.specular * spec * Material.specular;

// 累加三个部分即可
vec3 resultColor = ambient + diffuse + specular;
```
##光照贴图
&emsp;&emsp;我们可以使用贴图来更加近似地模拟真实环境，同样为了更加容易区分出物体表面的漫反射以及镜面反射，我们可以使用漫反射贴图和镜面反射贴图来控制反射的颜色；同样对于某些自发光的物体，你还可以使用mission纹理来模拟自发光。 
![此处输入图片的描述][12]


上面是一张漫反射纹理，我们再获取对应坐标的位置后，使用纹理颜色替换材质颜色进行漫反射计算即可。
![此处输入图片的描述][13]


与漫反射贴图类似，我们可以将镜面反射纹理颜色乘到原来的镜面反射颜色部分。

##投光物
顾名思义就是将光线投出去的东西，也即光源。

1. 生活中所有的光源都是点光源，太阳就是无数的点光源组成的，因为距离很远，我们可以把太阳看成一个点光源。假设光源上两个点距离Dis，光源到物体距离R,那么这个两个点光源到该物体的夹角为Dis/R，由于R很大，所以夹角近似为0，可以认为从两个光源来的光线重合，也即可以认为是一个点光源。可以得出结论，当距离足够远时，可以把多个点光源看成一个**点光源**。
同样，当半径非常大时，圆上两个距离很近的点，这两个点之间的角度差（Dis/R）近似为0，也即圆心到这两点的直线夹角为0，也即不相交（以上都是近似）。可以得出结论，当距离足够远时，可以把点光源看成**平行光源**。
2. 考虑到真实环境中的光线衰减（散射、折射、频率降低等等），我们可以使用线性函数近似模拟这个过程，通过指定多项式的因子，来控制衰减速度变化曲线，通常我们使用二次就足够了，公式：1.0 / (1.0 + a * D + b * D^2)，D是到光源的距离，通过指定不同的a、b值，我们就可以模拟不用的衰减变化曲线。
3. 经过遮挡物，遮挡物后的光线会被阻挡，我们使用**聚光**来模拟这种情况（类似手电筒），也即一个点光源加上限制的光源散射角度范围。一般情况下可投影的范围是个圆形，这时只需要比较片元到光源的方向是否在限制角度内即可，但是也存起可投影范围不是圆形的情况，例如方形，那么这个时候，可以控制模板测试？
4. 但是真实情况手电筒并不是点光源，当距离比较近时，半影区会比较明显，这时可以使用近似方法模拟这种半影，例如半影区一次线性变化（角度or余弦值？）等等。

##多光源
灵活利用之前提到的光源即可。


#高级Opengl渲染管线之测试（一）
##深度测试
&emsp;&emsp;深度测试默认是禁用的，所以如果要启用深度测试的话，我们需要用GL_DEPTH_TEST选项来启用它：

    glEnable(GL_DEPTH_TEST);
&emsp;&emsp;当它启用的时候，如果一个片段通过了深度测试的话，OpenGL会在深度缓冲中储存该片段的z值；如果没有通过深度缓冲，则会丢弃该片段。如果你启用了深度缓冲，你还应该在每个渲染迭代之前使用GL_DEPTH_BUFFER_BIT来清除深度缓冲，否则你会仍在使用上一次渲染迭代中的写入的深度值：

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
&emsp;&emsp;可以想象，在某些情况下你会需要对所有片段都执行深度测试并丢弃相应的片段，但不希望更新深度缓冲。基本上来说，你在使用一个只读的(Read-only)深度缓冲。OpenGL允许我们禁用深度缓冲的写入，只需要设置它的深度掩码(Depth Mask)设置为GL_FALSE就可以了：

    glDepthMask(GL_FALSE);

###深度测试函数
&emsp;&emsp;OpenGL允许我们修改深度测试中使用的比较运算符。这允许我们来控制OpenGL什么时候该通过或丢弃一个片段，什么时候去更新深度缓冲。我们可以调用glDepthFunc函数来设置比较运算符（或者说深度函数(Depth Function)）：

    glDepthFunc(GL_LESS);

| 函数 | 描述 |
| :------: | :----------: |
|GL_ALWAYS|永远通过深度测试|
|GL_NEVER|	永远不通过深度测试|
|GL_LESS|	在片段深度值小于缓冲的深度值时通过测试|
|GL_EQUAL|	在片段深度值等于缓冲区的深度值时通过测试|
|GL_LEQUAL|	在片段深度值小于等于缓冲区的深度值时通过测试|
|GL_GREATER	|在片段深度值大于缓冲区的深度值时通过测试|
|GL_NOTEQUAL|	在片段深度值不等于缓冲区的深度值时通过测试|
|GL_GEQUAL|	在片段深度值大于等于缓冲区的深度值时通过测试|

###深度值精度
&emsp;&emsp;深度缓冲包含了一个介于0.0和1.0之间的深度值，它将会与观察者视角所看见的场景中所有物体的z值进行比较。观察空间的z值可能是投影平截头体的近平面(Near)和远平面(Far)之间的任何值。我们需要一种方式来将这些观察空间的z值变换到[0,1]范围之间，其中的一种方式就是将它们线性变换到[0, 1]范围之间。
![此处输入图片的描述][15]

&emsp;&emsp;在实践中是几乎永远不会使用这样的线性深度缓冲(Linear Depth Buffer)的。要想有正确的投影性质，需要使用一个非线性的深度方程，它是与 1/z 成正比的。它做的就是在z值很小的时候提供非常高的精度，而在z值很远的时候提供更少的精度。
![此处输入图片的描述][16]
![此处输入图片的描述][17]


&emsp;&emsp;可以看到，深度值很大一部分是由很小的z值所决定的，这给了近处的物体很大的深度精度。这个（从观察者的视角）变换z值的方程是嵌入在投影矩阵中的，所以当我们想将一个顶点坐标从观察空间至裁剪空间的时候这个非线性方程就被应用了。

##模板测试
FragShader —— Stencil Test —— Depth Test

#高级OpenGL渲染管线之混合与面剔除（二）

详细请阅读：https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/03%20Blending/

##混合
用于混合多种颜色，适用于不透明的场景；

如果是全透明时可以在shader中使用discard字段，显式表示不进行后面的处理；
```
glEnable(GL_BLEND); // 开启混合测试
glBlendFunc(GLenum sfactor, GLenum dfactor); // 设置源因子和目标因子
glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); // 分别设置RGB分量和Alpha的源因子和目标因子
glBlendEquation(GLenum mode); // 设置源颜色 与 目标颜色之间的算术运算
```

&emsp;&emsp;源颜色指源自纹理的颜色向量，目标颜色指存储在颜色缓冲中的颜色向量。

&emsp;&emsp;需要特别注意混合与深度测试之间的关系。因为深度测试不关心透明度，如果先绘制了近处的透明物体，那么深度缓冲会被更新为一个比较小的值，此后如果再绘制透明物体后的物体时，将不能通过深度测试，导致透明物体后的物体不显示。对于使用了混合的场景，我们应该使用下面的步骤：

1. 先绘制所有不透明的物体。
2. 对所有透明的物体排序。
3. 按顺序绘制所有透明的物体。

或者在绘制透明物体时关闭深度测试，
1. 建议使用glDepthMask(GL_FALSE)，防止更新深度缓冲。
2. 使用glEnable(GL_DEPTH_TEST)时仍然可以写入深度缓冲，所以不推荐。

##面剔除
&emsp;&emsp;在顶点着色器之后，光栅化阶段，会执行面剔除。因为我们需要先确定图元与相机的位置与角度关系，才能确定正反面。默认情况下，正面为顶点逆时针顺序，朝向我们的面，但是我们可以自定义正面。

     glFrontFace(GL_CW); // 将顺时针的面定义为正向面，默认 GL_CCW
我们开启面剔除，并指定剔除正面或者背面或者两者。

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
&emsp;&emsp;想象一下，你再绘制立方体的六个面时，你需要保证六个面的正面都是朝外，这么当我们从一个方向看向立方体时，光线穿过近面然后再穿过远面，近面的正向面向你，而因为光线是从立方体内穿过远面，远面是反面对着你。

&emsp;&emsp;其实当你看向一个立方体时，你最多只能同时看到3个面，而那些你看不到的面都是反面。那么我们就可以通过面剔除去掉那些不需要渲染的面，避免片元着色器做无用功。但是近面是透明物体时，这时就不能剔除反面了。

#高级OpenGL渲染管线之帧缓冲（三）
&emsp;&emsp;帧缓冲包括之前学的：用于写入颜色值的颜色缓冲、用于写入深度信息的深度缓冲和允许我们根据一些条件对齐特定片段的模板缓冲。他们都存储在内存中，我们也可以定义自己的帧缓冲。

&emsp;&emsp;在绑定到GL_FRAMEBUFFER目标之后，所有的读取和写入帧缓冲的操作将会影响当前绑定的帧缓冲。我们也可以使用GL_READ_FRAMEBUFFER或GL_DRAW_FRAMEBUFFER，将一个帧缓冲分别绑定到读取目标或写入目标。绑定到GL_READ_FRAMEBUFFER的帧缓冲将会使用在所有像是glReadPixels的读取操作中，而绑定到GL_DRAW_FRAMEBUFFER的帧缓冲将会被用作渲染、清除等写入操作的目标。大部分情况你都不需要区分它们，通常都会使用GL_FRAMEBUFFER，绑定到两个上。

一个完整的帧缓冲需要满足以下的条件：

- 附加至少一个缓冲（颜色、深度或模板缓冲）。
- 至少有一个颜色附件(Attachment)。
- 所有的附件都必须是完整的（保留了内存）。
- 每个缓冲都应该有相同的样本数。
- 在完整性检查执行之前，我们需要给帧缓冲附加一个附件。附件是一个内存位置，它能够作为帧缓冲的一个缓冲，可以将它想象为一个图像。当创建一个附件的时候我们有两个选项：纹理或渲染缓冲对象(Renderbuffer Object)。

##纹理附件
&emsp;&emsp;当把一个纹理附加到帧缓冲的时候，所有的渲染指令将会写入这个纹理中，可以认为她就是一个普通的颜色/深度或模板缓冲。为帧缓冲创建一个纹理和创建一个普通的纹理差不多：
```
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
```
主要的区别就是，我们将维度设置为了屏幕大小（尽管这不是必须的），并且我们给纹理的data参数传递了NULL。对于这个纹理，我们仅仅分配了内存而没有填充它。填充这个纹理将会在我们渲染到帧缓冲之后来进行。同样注意我们并不关心环绕方式或多级渐远纹理，我们在大多数情况下都不会需要它们。

如果你想将你的屏幕渲染到一个更小或更大的纹理上，你需要（在渲染到你的帧缓冲之前）再次调用glViewport，使用纹理的新维度作为参数，否则只有一小部分的纹理或屏幕会被渲染到这个纹理上。

1. 除了颜色附件（GL_COLOR_ATTACHMENT0）外，我们还可以附加深度（GL_DEPTH_ATTACHMENT）和模板缓冲(GL_STENCIL_ATTACHMENT)纹理到帧缓冲对象中，或者，我们可以将深度和模板放到一个附件（GL_DEPTH_STENCIL_ATTACHMENT）中。

2. 对于颜色纹理附件，我们使用glTexImage2D来分配内存空间，对于深度和模板缓冲，我们使用glRenderbufferStorage来分配空间。

3. 颜色纹理attach使用的是glFramebufferTexture2D, 深度和模板则使用glFramebufferRenderbuffer。

##渲染到纹理步骤
1. 生成纹理缓冲
2. 绑定到当前纹理缓冲
3. 生成纹理，并将该纹理attach到纹理缓冲
4. 如果需要，生成深度和模板缓冲附件并attach
5. 要绘制到该纹理缓冲时只需要激活该纹理缓冲即可 glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
6. 执行绘制
7. 我们访问绑定到该纹理缓冲上的纹理即可获得绘制得到的纹理。
8. 因为东西只绘制在新建的纹理缓冲中，所以记得切换回默认帧缓冲，并将纹理绘制到默认缓冲上，glBindFramebuffer(GL_FRAMEBUFFER, 0);

##后期处理
在上面的第6步后，我们就得到了屏幕截图纹理，有了这个纹理，我们可以做很多单纯使用shader无法完成的事情。因为shader中我们只关注单个顶点或单个片元（像素）的位置与颜色，我们并不关心他与周围顶点或片元之间的关系。基于这个屏幕截图纹理，我们可以完成例如灰度、模糊、锐化、边缘检测等很多操作。当然，灰度也可以在shader里完成，不同的是，我们可以单独写一个灰度的shader，来对整个屏幕的场景进行灰度，而避免了在所有需要灰度的物体的shader里加上灰度的代码。

##核效果
&emsp;&emsp;我们这里暂时只关注像素与紧邻的像素的关系，我们将其的乘因子的矩阵单独列出来，称作核，它有下面的这种形式。
![此处输入图片的描述][18]


&emsp;&emsp;但是需要注意的是，我们保证所有因子的和为1，否则可能造成变亮或变暗，这个和我们的提高亮度shader和减小亮度shader是类似的。
```
const float offset = 1.0 / 300.0;  

void main()
{
    -- 只是为了更加方便来索引像素周围的像素
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        -- 这里只是单纯的乘以因子，然后累加
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
```

&emsp;&emsp;我们只是单纯地把核用来存储乘因子，然后将每个元素乘以乘因子累加得到最终的结果。我们当然可以使用其他更复杂的算法，只不过，上面这种简单的算法已经能够充分表现各种效果了，而我们只需要调整核中的各个因子的比例即可。最重要的原因是这样更简单性能更好。

&emsp;&emsp;上面的例子是锐化，从核因子的比例，我们可以看出，使用该核因子的目的是为了放大目标像素与周围像素的不同，而我们称呼这个不同造成的现象为锐化。同理，如果要制作模糊的核，因为模糊的本质是目标像素和周围的像素差别变小，那么我们可以写出下面的核。
![此处输入图片的描述][19]


&emsp;&emsp;上面的模糊核没有负数，也即只是单纯求平均。我们可以改变中间因子与周围因子的比例，来控制模糊的系数，当中间为1，周围均为0时，也就是不模糊了。

边缘检测可以理解为更加锐化。

#高级OpenGL渲染管线之立方体贴图（四）
&emsp;&emsp;可以想象一下，如果我们有一张可以将摄像机整个包住的大纹理，那么不管摄像机怎么转都能够看到对应的像素。很容易联想到，我们可以用它来实现不会发生变化的场景，例如远处的山，填上的太阳，不会动的云等等。

&emsp;&emsp;但是，这个纹理应该是怎样的呢？OpenGL里有立方体贴图，他可以实现这个功能。为什么不用球，我认为应该是立方体足够使用了，并且切图更加方便，使用起来也方便。虽然可能造成面中心的点与边上的点到球心的距离不一样导致的显示效果不一致，但是只要分辨率足够，我们是很难看出那种差距的。

##创建立方体贴图
&emsp;&emsp;和创建普通的纹理一样，我们创建，然后绑定立方体贴图。

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

&emsp;&emsp;不同的是，立方体贴图有六个面，我们需要为六个面分别加载纹理数据。这六个面分别是

|纹理目标|	方位|
| :--: | :--: |
|GL_TEXTURE_CUBE_MAP_POSITIVE_X	|右
|GL_TEXTURE_CUBE_MAP_NEGATIVE_X	|左
|GL_TEXTURE_CUBE_MAP_POSITIVE_Y	|上
|GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	|下
|GL_TEXTURE_CUBE_MAP_POSITIVE_Z	|后
|GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	|前
&emsp;&emsp;幸运的是，GL_TEXTURE_CUBE_MAP_POSITIVE_X 下面的值都与上一个偏移1，我们可以使用循环访问。

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
&emsp;&emsp;不要被GL_TEXTURE_WRAP_R吓到，它仅仅是为纹理的R坐标设置了环绕方式，它对应的是纹理的第三个维度（和位置的z一样）。我们将环绕方式设置为GL_CLAMP_TO_EDGE，这是因为正好处于两个面之间的纹理坐标可能不能击中一个面（由于一些硬件限制），所以通过使用GL_CLAMP_TO_EDGE，OpenGL将在我们对两个面之间采样的时候，永远返回它们的边界值。

1. 在shader里 ，立方体贴图的采样器是samplerCube，采样时的第二个参数为点的方向。

2. 在绘制立方体贴图时，八个顶点的位置也就是它在摄像机坐标系的的方向。

3. 因为天空盒一直以摄像机为原点，所以不需要模型矩阵。

4. 需要注意的一点是，我们假设天空盒是无限大的，所以摄像机的运动对天空盒的场景不应该有位移变化，所以，传入的摄像机矩阵不应该包含摄像机的位移。
```
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
```

##优化
&emsp;&emsp;天空盒非常大，每个绘制循环都把天空盒所有面都画到难免浪费性能，所以我们可以使用提前深度测试丢弃不需要绘制的天空盒。如果我们先绘制天空盒再绘制物体，就会绘制天空盒的所有片元。

&emsp;&emsp;提前深度测试是指在片元着色器之前执行的深度测试，一般我们说的深度测试是指在片元着色器—模板测试—深度测试-Blend 中的深度测试。也就是说，只要我们先绘制了其他物体，接下来绘制天空盒的时候就会自动使用提前深度测试。

&emsp;&emsp;由于天空盒无限大，也即无限远，所以我们需要将天空盒像素的点的Z值置为1.0表示无限远，gl_Position = pos.xyww;（w等于1.0）.

&emsp;&emsp;由于先绘制了物体，这时候深度缓冲中物体之外的地方都是无限远（1.0），而天空盒的z值又都是无限远1.0，所以需要调整深度函数为GL_LEQUAL（小与等于），这样天空盒就会绘制上了。

##环境映射
&emsp;&emsp;这里的环境映射包括反射和折射，实现方法是：假设摄像机在物体某个像素处看到的颜色，那么方向由反射和折射角确定，反射和折射角则由摄像机坐标和物体表面的像素坐标，法线，反射（折射）率确定。

##动态环境贴图
**目的：**如果场景中有多个物体，这些多个物体都是镜面的，基于之前的环境映射，那么只能将天空盒的纹理映射到镜面上，那么镜面上肯定不会有其他的物体。为了解决这个问题。

**策略：**在绘制那个镜面前，使用帧缓冲，先将其他物体和天空盒绘制出来，将绘制在帧缓冲中的纹理当做天空盒的纹理。

**缺点：**当有多个镜面物体时仍然难以在镜面中显示镜面物体。需要为使用环境贴图的物体渲染场景6次，性能消耗大。


#高级OpenGL渲染管线之高级数据及高级GLSL（五）

##高级数据
&emsp;&emsp;我们之前定义顶点属性时都是一次性写入顶点数据，其实，我们还可以使用先分配内存再填数据的方式来写入顶点数据。

    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(data), &data);
&emsp;&emsp;但是在使用glBufferSubData写入数据之前，仍然需要先使用glBufferData，但是参数中数据指针为空，这样就可以只分配内存空间而不填入数据。

&emsp;&emsp;另外我们还可以使用glMapBuffer来获取缓冲数据的指针，然后直接使用内存拷贝的方式复制数据，但是记得使用完毕后删除指针变量。获取指针时也需要指定以后是用来读取还是写入。使用这种方式，我们能够直接将数据从文件导入到缓冲，而不需要创建临时空间。
```
float data[] = {
  0.5f, 1.0f, -0.35f
  ...
};
glBindBuffer(GL_ARRAY_BUFFER, buffer);
// 获取指针
void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
// 复制数据到内存
memcpy(ptr, data, sizeof(data));
// 记得告诉OpenGL我们不再需要这个指针了
glUnmapBuffer(GL_ARRAY_BUFFER);
```

###分批顶点属性
&emsp;&emsp;我们之前是将顶点坐标、颜色、纹理坐标看成一个顶点数据，所有顶点数据依次排列得到所有数据。这样相邻顶点间的相同类型数据的间隔是相同的。我们还可以使用另外一种方式设置数据，将所有的顶点坐标放在一起，所有的颜色放在一起，也即将所有相同类型的数据放在一起。这时我们就需要用到glBufferSubData的偏移参数了，同理在定义顶点属性时，stride和最后一个偏移参数也会有所不同。
```
float positions[] = { ... };
float normals[] = { ... };
float tex[] = { ... };
// 填充缓冲
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));  
glVertexAttribPointer(
  2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));
```
###复制缓冲
&emsp;&emsp;我们可以使用glCopyBufferSubData复制任意两个缓冲的数据，前提是我们需要填入readtarget和writetarget，如果两者类型不一样，我们可以轻易知道怎么复制，但是如果两者类型一样呢，这是会因为找不到具体要从哪个缓冲对象复制到哪个缓冲对象而产生矛盾。

    void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
&emsp;&emsp;我们可以分别将两个缓冲对象绑定到GL_COPY_READ_BUFFER和GL_COPY_WRITE_BUFFER类型上，这时复制时就能够明确是要操作哪两个对象了。同样，只要绑定了GL_COPY_READ_BUFFER和GL_COPY_WRITE_BUFFER，我们可以任意使用glCopyBufferSubData，只要保证readtarget和writetarget不同时为GL_COPY_READ_BUFFER或GL_COPY_WRITE_BUFFER即可。



##高级GLSL
###GLSL内建变量
&emsp;&emsp;GLSL里有很多内建变量，我们可以直接使用它们。我们之前已经用到了gl_Position，他是顶点着色器的输出，表示经过顶点着色器的顶点位置。

###顶点着色器变量
- gl_Position：vec4，点的位置
- gl_PointSize：float，点的大小，当我们绘制的图元是GL_POINTS时，我们改变这个值才有效。默认不允许改变，使用glEnable(GL_PROGRAM_POINT_SIZE);//允许顶点着色器修改。
- gl_VertexID：int，当前点的索引，输入变量，只有当使用glDrawElements绘制时才有效。使用glDrawArrays绘制时存储的值为已处理顶点数量。

### 片段着色器变量
- gl_FragCoord：vec3，表示片元在屏幕空间的坐标，包含深度。
- l_FrontFacing：bool，当前片元是否正面。可以输入正反面纹理，通过这个变量显示正反面不同纹理。
- gl_FragDepth：float，当前片元深度值，可写；gl_FragCoord中的深度值只读。如果在片段着色器中对该变量进行写入，那么提前深度测试（在片元着色器之前执行）将不可用，因为我们不能确定片元着色器会写入什么值。但有一种方法可以确定深度值的范围。
    layout (depth_<condition>) out float gl_FragDepth;
我们可以通过限定gl_FragDepth可能变化的趋势，从而保证提前深度测试可用。例如，如果我们的深度函数是Less，那么当我们的限定条件是greater时，那么就说明，只要提前深度测试通过了，在限定条件下，不管深度值怎么变，都是不影响深度测试结果的，那么提前深度测试是可用的。我们可以使用的条件有any、greater、less、unchanged。但这个特性只有OpenGL4.2以上才支持。

```
#version 420 core // 注意GLSL的版本！
out vec4 FragColor;
layout (depth_greater) out float gl_FragDepth;

void main()
{             
    FragColor = vec4(1.0);
    gl_FragDepth = gl_FragCoord.z + 0.1;
}  
```

### 接口块
&emsp;&emsp;我们经常会遇到在顶点着色器和片元着色器间传数据时，需要定义很多in、out变量，当我们需要发送结构更加复杂的数组或者结构体时，代码就变得复杂起来。所以，OpenGL提供了接口块的方式来链接两个着色器间的变量。

```
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec2 TexCoords;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);    
    vs_out.TexCoords = aTexCoords;
}  


#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture;

void main()
{             
    FragColor = texture(texture, fs_in.TexCoords);   
}
```

&emsp;&emsp;在上面的例程中定义的接口块名称为VS_OUT，区别在于是in还是out。在顶点着色器中有个别名vs_out（暂且叫别名或者实例名），片元着色器中有个别名fs_in，他们通过VS_OUT这个类型名连接起来，有了别名，我们在不同的着色器里使用时也避免了歧义。

###Uniform缓冲对象
&emsp;&emsp;我们也可以为uniform创建缓冲对象，这样，当我们需要往不同的shader内注入大量相同的Uniform变量时，我们就不必要一个个地设置了。例如下面例程中的投影矩阵和摄像机矩阵。模型矩阵没有放在里面是因为它可能在不同物体间经常变化。

```
#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```

###Uniform块布局方式
&emsp;&emsp;前面例程定义shader内的uniform变量，但是程序如何将该变量传递进去呢？首先，我们需要考虑Matrices内的多个变量在内存中的布局方式。需要注意的是，GLSL的变量bool、float、int都是4字节。例程中的布局方式是std140，这是一种确定变量大小的布局方式，和文件的对齐方式类似。这种布局方式就为确定每个变量的位置提供了条件，我们可以手动计算出每个变量的偏移。

&emsp;&emsp;每个变量都有一个基准对齐量（Base Alignment），他是指这个变量所占的内存空间（和实际大小不一样，例如bool只有一位但是占用了4字节）。我们将每个变量按照定义的顺序排列好，每个变量占用空间就是基准对齐量的最小整数倍，那么就可以算出每个变量的偏移了。我们使用N来表示4个字节，也即一个简单类型所占的空间大小。更多规则。

|类型|	布局规则|
|:-:|:-:|
|标量，比如int和bool|	每个标量的基准对齐量为N。
|向量	|2N或者4N。这意味着vec3的基准对齐量为4N。
|标量或向量的数组|	每个元素的基准对齐量与vec4的相同。
|矩阵	|储存为列向量的数组，每个向量的基准对齐量与vec4的相同。
|结构体	|等于所有元素根据规则计算后的大小，但会填充到vec4大小的倍数。

```
layout (std140) uniform ExampleBlock
{
                     // 基准对齐量       // 对齐偏移量
    float value;     // 4               // 0 
    vec3 vector;     // 16              // 16  (必须是16的倍数，所以 4->16)
    mat4 matrix;     // 16              // 32  (列 0)
                     // 16              // 48  (列 1)
                     // 16              // 64  (列 2)
                     // 16              // 80  (列 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
}; 
```

**这里需要注意的是，虽然vec3只用到了3 * 4字节，但是所有向量的基准对齐量都是2N或者4N，所以vec3只能为4N。**

***上面例程的vecs对齐偏移量为什么是16 ，因为向量的偏移必须是4N的倍数？***

&emsp;&emsp;另外我们还可以使用一个叫做共享（Shared）布局的Uniform内存布局方式。他的思想是动态调整每个变量，这样的话就可以减少内存浪费。但是这样我们就不能知道每个变量在哪里，所以每次使用前都必须通过glGetUniformIndices这样的函数来查询变量的位置。

###使用Uniform缓冲
&emsp;&emsp;上面已经介绍完了Uniform缓冲在shader内部的定义以及数据的内存布局，接下来就该讲如何通过程序，将每个变量根据之前定义的布局放置到正确的位置。下面的代码创建一个Uniform缓冲，并分配好内存，但是未写入数据，写输入需要使用glBufferSubData。

```
unsigned int uboExampleBlock;
glGenBuffers(1, &uboExampleBlock);
glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // 分配152字节的内存
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

&emsp;&emsp;到这里，我们还是不能将缓冲对象与shader里的uniform块绑定起来，所以下面需要使用OpenGL的绑定点（Binding Point），我们可以把它理解成一个纽带，一头绑Shade的Uniform块，一头绑Uniform缓冲对象，他们是多对多的关系。
![此处输入图片的描述][20]


&emsp;&emsp;ShaderA 和ShaderB的Matrices，他们都需要uboMatrices的数据。下面的例程定义的是将ShadeA的Lights Uniform块绑定到绑定点，然后将uboLights Unform缓冲对象绑定到绑定点，这样就实现了shader和UBO的数据互通。使用glBindBufferRange接口还可以实现只绑定Ubo的部分数据，这样一个ubo里就可以包含多个Uniform块的数据，需要用时分别取用即可。

```
unsigned int lights_index = glGetUniformBlockIndex(shaderA.ID, "Lights");   
glUniformBlockBinding(shaderA.ID, lights_index, 2);

glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock); 
// 或
glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);
```

从OpenGL4.2开始，我们还可以使用下面的方式手动指定绑定点，这样就不需要在代码里绑定了。

    layout(std140, binding = 2) uniform Lights { ... };
&emsp;&emsp;别忘了，我们还可以往UBO中添加数据，只要你想，你可以在任何时候使用glBufferSubData往缓冲里添加数据，但是别忘了加偏移。注意，使用int来存需要放入shader的bool数据，例如：int b = true;

&emsp;&emsp;使用Uniform还可以减小OpenGL对Uniform数量（查询 GL_MAX_VERTEX_UNIFORM_COMPONENTS）的限制。

#高级OpenGL渲染管线之几何着色器（六）
&emsp;&emsp;几何着色器在顶点着色器和片元着色器中间。从下面的OpenGL渲染流程图可以看出，渲染的流程为顶点着色器—图元装配—视口剔除—光栅化—片元着色器。而几何着色器在光栅化阶段，但是在光栅化之前执行。
![此处输入图片的描述][21]


&emsp;&emsp;几何着色器的输入是一个图元的所有顶点，输出是图元类型和所有顶点。

```
#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
```

&emsp;&emsp;上面的例程中，我们使用layout修饰从输入输出，输入类型定义为points，我们还可以使用下面这些类型。

- points：绘制GL_POINTS图元时（1）。
- lines：绘制GL_LINES或GL_LINE_STRIP时（2）
- lines_adjacency：GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY（4）
- triangles：GL_TRIANGLES、GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN（3）
- triangles_adjacency：GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY（6）

&emsp;&emsp;输出类型接受下面三种类型：

- points
- line_strip
- triangle_strip

&emsp;&emsp;同时我们还需要在输出中使用max_vertices来修饰片元的最大顶点数。有了顶点数我们就可以使用EmitVertex函数和EndPrimitive函数来绘制我们的图元，EmitVertex函数是绘制一个顶点，EndPrimitive函数表示停止之前的绘制并合成图元。上面的例子中，连续线段的最大顶点数是2，所以绘制两个点就够了。

&emsp;&emsp;有了上面的功能，我们就可以任意修改图元的顶点，甚至修改图元的类型。我们可以输入一个顶点，输出连续三角形，那么，最后的结果是，之前定义的顶点出就会绘制出我们修改后的连续三角形。

```
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

void build_house(vec4 position)
{    
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}
```

&emsp;&emsp;上面的例子展示的是将输入的顶点转化为一个小房子状的连续三角形（triangle_strip），小房子一共3个三角形，那么最大定点数是3 + 2。我们以输入的顶点坐标为初始位置，以不同的位置偏移发射小房子的5个顶点，最后组成图元就有结果了。

&emsp;&emsp;有一点需要注意的是，几何着色器的输入图元类型是由我们在渲染循环里调用glDrawArray和glDrawElement是设置的绘制模式对应的，也即如果要让上面那个几何着色器生效，就需要在绘制时使用GL_POINTS。
![此处输入图片的描述][22]


 还有一点需要注意，顶点着色器与几何着色器间的数据传输，我们需注意它的使用方式，以接口块为例。

```
// 顶点着色器接口块定义
out VS_OUT {
    vec3 color;
} vs_out;

// 几何着色器接口块定义
in VS_OUT {
    vec3 color;
} gs_in[];
```

&emsp;&emsp;上面的例子中给出了两处的定义方式，需要注意的地方在几何着色器中定义的是一个数组。这是因为顶点着色器处理的是单个顶点理，它的输入是一个顶点的位置。而几何着色器处理的是一个图元，他输入的是一个图元的一组顶点。

&emsp;&emsp;我们可以在几何着色器中修改顶点颜色，需要注意的是，只要我们修改了输出到片段着色器的颜色，那么该几何着色器后面绘制的顶点颜色都是修改后的颜色，除非再次修改颜色。

```
fColor = gs_in[0].color; 
gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下 
EmitVertex();   
gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
EmitVertex();
gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
EmitVertex();
gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
EmitVertex();
gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
fColor = vec3(1.0, 1.0, 1.0);
EmitVertex();
EndPrimitive();
```

##爆破物体
&emsp;&emsp;使用几何着色器，将顶点沿着法线方向运动一段距离。我们主动传入法向量，或者自己计算法向量，已知三角形的三个顶点计算法向量是很简单的，只是需要注意规范化。
![此处输入图片的描述][23]


##法向量可视化
&emsp;&emsp;顾名思义就是看得见法向量，思想是，先绘制一遍原物体，然后再绘制出法线，绘制法线时是用原物体的顶点数据，增加几何着色器，在几何着色器里把法线画出来即可。
![此处输入图片的描述][24]

```
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线
}
```

&emsp;&emsp;上面的例程需要注意的是，我们输入图元是三角形，输出是连续线段，最大顶点是6，而代码里在EndPrimitive之前只发射了两个顶点，所以最大顶点数设置为2就足够了。

#高级OpenGL渲染管线之实例化与抗锯齿（七）

##实例化
&emsp;&emsp;当场景中有大量相似（形状一样，只是角度、位置、缩放不一样）的物体时，如果继续使用glDrawArray或glDrawElement来绘制每一个物体时，大量的性能将会被浪费在和设置OpenGL上下文和与GPU通讯上，因为大量的物体可以使用相同的顶点和模型矩阵，所以我们没必要每一次绘制都设置OpenGL上下文和GPU通讯，我们只需要一次将生成不同物体间不同属性的必要数据导入，然后绘制所有相同的物体即可。

&emsp;&emsp;基于glDrawArrayInstance或glDrawElementInstance，我们可以使用一份顶点数据在一个draw周期内绘制多个物体。GLSL的内建变量gl_instance表示当前正在绘制第几个实例。

&emsp;&emsp;我们可以将所有实例的属性传入着色器，然后使用gl_Instance当做索引，设置当前实例的位置、旋转、大小等等。

###实例化数组
&emsp;&emsp;其实也可以使用VBO而不是uniform传入实例的属性。

```
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main()
{
    gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
    fColor = aColor;
}
```
```
unsigned int instanceVBO;
glGenBuffers(1, &instanceVBO);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);

// 设置属性
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
glBindBuffer(GL_ARRAY_BUFFER, 0);   
glVertexAttribDivisor(2, 1);
```

&emsp;&emsp;shader内部定义和一般的VBO使用并无不同，创建、绑定、设置VBO数据也和其他的一样。不同的是在最后，我们需要使用glVertexAttribDivisor设置每个属性的更新时机。实例的数据存储在第3个属性内，因为第一个属性（顶点）和第二个属性（颜色）都是每个顶点更新一次，所以使用默认的即可，而第三个属性是必须每个实例更新一次的，所以这里使用glVertexAttribDivisor(2, 1)来声明。第一个参数时属性位置，第二参数为0表示每个顶点更新，也是默认值，为1时表明每个实例更新，为N时表明每N个实例更新。


##抗锯齿
&emsp;&emsp;锯齿的表现是线段在视觉上不连续（如下图），我们称这种现象为走样（Aliasing），他的根本原因是存储线段中点的坐标位置的分辨率不够。那么如何减弱这种现象呢？虚化！线段占用在某个像素的控件越多，那么像素颜色就越接近线段的颜色。
![此处输入图片的描述][25]


&emsp;&emsp;在抗锯齿方面，我们有很多方案，比如SSAA（超采样， Super Sample Anti-aliasing），也即使用比正常分辨率更高的分辨率。显而易见，这里会产生非常大的性能消耗。另外还有MSAA（Multisampling），增加像素的采样点，颜色变化更接近线段占像素的面积。如下如所以，普通情况下的像素采样点只有一个，那么，当倾斜角度比较大时，很可能线段占多个像素的面积都不够1/2或者多于1/2，那么表现是连续几个像素都点亮或者熄灭，那么就产生了锯齿，增加采样点时可以减弱这种影响。
![此处输入图片的描述][26]


&emsp;&emsp;注意，虽然有4个采样点，但是片段着色器最多执行一次。因为只要覆盖了一个采样点，我们就需要使用片段着色器算出该片段被完整覆盖的颜色，然后按照采样点被覆盖的比例减小颜色即可。

###OpenGL的MSAA
我们可以使用下面的代码开启MSAA。

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

如果我们需要离屏MSAA，那么我们就必须自己实现多重采样。

###多重采样纹理附件
&emsp;&emsp;创建纹理目标时，使用glTexImage2DMultisample代替glTexImage2D，纹理目标是GL_TEXTURE_2D_MULTISAPLE。第二个参数是样本数，我们一般使用4个，就和上面的图片一样。如果最后一个参数为GL_TRUE，图像将会对每个纹素使用相同的样本位置以及相同数量的子采样点个数。

&emsp;&emsp;我们使用glFramebufferTexture2D将多重采样纹理缓冲附加到帧缓冲上，这里的纹理类型是GL_TEXTURE_2D_MULTISAMPLE。

```
glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
```

###渲染到多重采样帧缓冲
&emsp;&emsp;配置好多重采样帧缓冲，绑定好了颜色、深度、模板缓冲。因为多重采样的图像比普通图像包含更多的信息，所以我么通常使用glBlitFramebuffer来转换，他能够将一个帧缓冲的某个区域复制到另一个帧缓冲中，并且将多重采样缓冲还原。下面是将多重采样帧缓冲放到屏幕上显示。

```
glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
```

&emsp;&emsp;我们不可以直接操作多重采样帧缓冲，如果我们有使用它的需求，必须将该多重采样帧缓冲转换到一个普通的帧缓冲，然后再操作其中的数据。

###自定义抗锯齿算法
&emsp;&emsp;如果我们需要自己定义几个采样点之间如何工作，那么我们就必须将多重采样的纹理传入。我们使用的是sampler2DMS。

    uniform sampler2DMS screenTextureMS;
    vec4 colorSample = texelFetch(screenTextureMS, TexCoords, 3);  // 第4个子样本

&emsp;&emsp;使用texelFetch获取采样点的颜色，TexCoords是该像素坐标，第三个参数是指第几个采样点。


#OpenGL高级光照Blinn-Phong模型和Gamma校正

##Blinn-Phong模型
&emsp;&emsp;我们之前使用的光照模型是Phone模型，但是该模型在低反光度时会存在不正常的高光区域。根本原因在于在该模型下计算反射光强度时，我们需要根据反射光与视线的夹角大小来决定光强，而当摄像机和光源在同一侧时，可能存在反射光与视线夹角大于90度，而我们使用cos函数来模拟越接近于0光强越大这个过程，那么当夹角大于90度时取值为负数就存在问题了。

&emsp;&emsp;Blinn-Phong模型就是取视线与反射光的中线，计算中线与法线的偏差角度，可以保证始终小于90度。
![此处输入图片的描述][27]
![此处输入图片的描述][28]
![此处输入图片的描述][29]

##Gamma校正
&emsp;&emsp;问题在于，人类感知的光线变化、代码里定义的光线变化、监视器展示的光线变化曲线是不一样的。一般代码里定义的光线变化是物体光量变化，它是一次线性的。
![此处输入图片的描述][30]

&emsp;&emsp;上图中第二行为不同线性光量下的亮度，第一个行为该光量下人眼察觉到的，可以得出，光量增加了一倍，人眼才能察觉到亮度增加了0.1。但是CRT的亮度刚好与人眼相反，电压线性变化时，光量以2.2为幂，指数函数变化。
![此处输入图片的描述][31]

&emsp;&emsp;当我们以物体光亮为基准实现光照时，有一个问题，我们在渲染时的光亮线性变化在CRT显示器上就是指数变化，这在复杂的光照计算时会显得不真实。为此，我们需要gamma校正。

&emsp;&emsp;Gamma校正(Gamma Correction)的思路是在最终的颜色输出上应用监视器Gamma的倒数。回头看前面的Gamma曲线图，你会有一个短划线，它是监视器Gamma曲线的翻转曲线。我们在颜色显示到监视器的时候把每个颜色输出都加上这个翻转的Gamma曲线，这样应用了监视器Gamma以后最终的颜色将会变为线性的。我们所得到的中间色调就会更亮，所以虽然监视器使它们变暗，但是我们又将其平衡回来了。

&emsp;&emsp;有两种在你的场景中应用gamma校正的方式：

- 使用OpenGL内建的sRGB帧缓冲：开启GL_FRAMEBUFFER_SRGB，告诉OpenGL每个后续的绘制命令里，在颜色存储到颜色缓冲之前，先校正sRGB颜色，sRGB的颜色空间大致对应于gamma2.2。开启以后，每次片段着色器运行后，都会将颜色校正后再传入帧缓冲。但是如果使用多个帧缓冲的话，可能会导致多次校正，所以最好是只在最后一步进行校正。

- 自己在像素着色器中进行gamma校正。

###sRGB纹理
&emsp;&emsp;如果像素是在使用SRGB的监视器上绘制，编辑的，那么我们也不用进行gamma校正，但是如果将它放到线性空间里显示，就会变暗。

###衰减
&emsp;&emsp;在不使用gamma校正时，二次衰减就太快了，因为gamma的指数又衰减了一次。当有gamma校正时一次就显得太慢了。


  [1]: https://blog.csdn.net/csxiaoshui/article/details/52785751
  [2]: https://img-blog.csdnimg.cn/20191218113451306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [3]: https://img-blog.csdnimg.cn/20191218113539940.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [4]: https://imgconvert.csdnimg.cn/aHR0cDovL2xlYXJub3BlbmdsLmNvbS9pbWcvZ2V0dGluZy1zdGFydGVkL2Nvb3JkaW5hdGVfc3lzdGVtcy5wbmc?x-oss-process=image/format,png
  [5]: https://img-blog.csdnimg.cn/20191021204307418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [6]: https://img-blog.csdnimg.cn/20191021204327351.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [7]: https://blog.csdn.net/csxiaoshui/article/details/52785751
  [8]: https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/02%20Creating%20a%20window/
  [9]: https://img-blog.csdnimg.cn/20191022223022661.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [10]: https://mp.csdn.net/postedit/102667561
  [11]: https://img-blog.csdnimg.cn/20191028160004363.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [12]: https://img-blog.csdnimg.cn/20191029202101527.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [13]: https://img-blog.csdnimg.cn/20191029200348557.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [14]: https://private.codecogs.com/gif.latex?1.0%20/%281.0%20&plus;%20a%20*%20D%20&plus;%20b%20*%20D%5E2%29
  [15]: https://img-blog.csdnimg.cn/20191031181251505.png
  [16]: https://img-blog.csdnimg.cn/2019103118144279.png
  [17]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzAxL2RlcHRoX25vbl9saW5lYXJfZ3JhcGgucG5n?x-oss-process=image/format,png
  [18]: https://img-blog.csdnimg.cn/20191106173308712.png
  [19]: https://img-blog.csdnimg.cn/20191106174524846.png
  [20]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzA4L2FkdmFuY2VkX2dsc2xfYmluZGluZ19wb2ludHMucG5n?x-oss-process=image/format,png
  [21]: https://img-blog.csdnimg.cn/20191218113539940.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2OTgzMTk1,size_16,color_FFFFFF,t_70
  [22]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzA5L2dlb21ldHJ5X3NoYWRlcl9ob3VzZXMucG5n?x-oss-process=image/format,png
  [23]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzA5L2dlb21ldHJ5X3NoYWRlcl9leHBsb3Npb24ucG5n?x-oss-process=image/format,png
  [24]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzA5L2dlb21ldHJ5X3NoYWRlcl9ub3JtYWxzLnBuZw?x-oss-process=image/format,png
  [25]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzExL2FudGlfYWxpYXNpbmdfem9vbWVkLnBuZw?x-oss-process=image/format,png
  [26]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA0LzExL2FudGlfYWxpYXNpbmdfc2FtcGxlX3BvaW50cy5wbmc?x-oss-process=image/format,png
  [27]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA1LzAxL2FkdmFuY2VkX2xpZ2h0aW5nX2NvbXBhcnJpc29uLnBuZw?x-oss-process=image/format,png
  [28]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA1LzAxL2FkdmFuY2VkX2xpZ2h0aW5nX292ZXJfOTAucG5n?x-oss-process=image/format,png
  [29]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA1LzAxL2FkdmFuY2VkX2xpZ2h0aW5nX2hhbGZ3YXlfdmVjdG9yLnBuZw?x-oss-process=image/format,png
  [30]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA1LzAyL2dhbW1hX2NvcnJlY3Rpb25fYnJpZ2h0bmVzcy5wbmc?x-oss-process=image/format,png
  [31]: https://imgconvert.csdnimg.cn/aHR0cHM6Ly9sZWFybm9wZW5nbC1jbi5naXRodWIuaW8vaW1nLzA1LzAyL2dhbW1hX2NvcnJlY3Rpb25fZ2FtbWFfY3VydmVzLnBuZw?x-oss-process=image/format,png