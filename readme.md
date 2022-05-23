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
    ./project1.exe
    ```

+ 目前已实现的功能
+ 面板上可以控制各个物体的材质、光照
+ 可以增加对某个物体实现拉升、旋转变换的按钮
+ 摄像机的视角可以实现改变
+ 场景物体可以增加