#### How to run
+ **Preliminary**
    + install cmake >= 3.15
    + C++ Compiler supports at least C++14
    
+ **Build and Compile**
    ```shell
    cd build
    cmake ..
    cmake --build . --parallel 8
    ```
    
+ **Run**
    ```shell
    cd bin/Debug
    ./2022CGFinalProject.exe
    ```

    ### 基础功能
    
    ##### 基本体素的绘制
    
    目前已实现：立方体和四面体的绘制
    
    待实现：圆柱、圆锥、多面棱柱、多面棱台
    
    实现方法：参考cube.h 和 cube.cpp 写法
    
    ##### 具有三维网格导入导出功能（obj格式）
    
    目前已实现：uno咖啡馆场景导入
    
    待实现：无（想有可以自己加）
    
    实现方法：
    
    1.同时在外层media文件和build中的media文件中加入相应的obj模型
    
    2.texture_mapping.cpp文件中加入模型路径
    
    3.加载模型中记得reset（new model对象）
    
    ##### 具有基本材质、纹理的显示和编辑能力
    
    目前已实现：panel面板中可以控制相应shader的参数，可以达到控制的目的
    
    待实现：可以自己编辑shader来点对点实现某些材质的实现
    
    实现方法：
    
    1.首先在texture_mapping.h中声明相应的shader
    
    2.在shading.cpp中完成相应shader的编写
    
    3.在texture_mapping.cpp中传入shader需要的参数（也有可能不需要）
    
    4.在相同文件中initshader
    
    5.shader->use()使用，然后model->draw（）
    
    ##### 具有几何变换功能
    
    目前已实现：旋转、平移、缩放
    
    待实现：无，可以选择三种功能加在一个物体上，也可以选择三种功能加在三个不同 的物体
    
    实现方法：
    
    1.改写要渲染物体的shader
    
    2.传入相应的参数
    
    3.imgui可以更改相应的参数
    
    4plus.可以直接仿照project6的模型来实现旋转，也可以仿照project2的模型来实现旋转
    
    ##### 场景漫游
    
    目前已实现：wasd 移动， fps相机风格
    
    待完成：Pan， Orbit， Zoom To Fit相机风格
    
    实现方式：自己查阅
    
    ##### 提供动画播放功能并且能够提供屏幕截取和保存功能
    
    目前已实现：无
    
    待完成：将一个obj序列一帧一帧的绘制
    
    实现方式：自己查阅
    
    
    
    ### 额外要求
    
    ##### NURBS曲面建模能力
    
    ##### 漫游时候可实现基于空间几何的碰撞检测
    
    ##### 光照明模型细化
    
    ##### 采用HTML5/IOS/Android移动平台实现
    
    ##### 构建基于此的完整三维游戏
    
    ##### 与增强现实应用结合
    
    ##### 具有一定的对象表达能力
    
    ​					