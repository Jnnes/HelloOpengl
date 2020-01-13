# HelloOpengl
## OpenGL学习
包含途径，库等已经配置好，项目可直接运行

编译运行步骤：

1. 安装VS2017
2. 更新git仓库的master到最新
3. 打开 HelloOpengl/HelloOpengl.sln 解决方案
4. 打开项目后，切换到 Debug Win32 编译运行
5. 实例化章节记得关闭显卡的垂直同步，否则最高刷新频率会被限制在显示器频率.



***注意：***由于后面的项目将glew库替换为glad，而且编写的Shader等类也修改为基于glad库，以有些来不及修改glew的项目会编译不通过，只需要按照其他项目替换为glad库即可。
注意不要少了往项目中添加glad.c文件
