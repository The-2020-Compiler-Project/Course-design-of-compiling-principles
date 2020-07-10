# 使用说明

## 工程打开/建立	

​	推荐使用`clion`打开CMakeLists工程文件，或者自行配置工程。

## 运行/更改main

​	根据`main.cpp`中的注释进行自己需要的修改。

​	直接运行会编译`Files`文件夹下相应的源文件，并将`token`、符号表、中间代码、优化后的中间代码、目标代码输出在`Files`文件夹下。

​	`cmd`模式：将编译后的`cmake-build-debug\complier.exe`放到`Files`文件夹下；配置系统环境加入`DosBox`所在的路径，配置`DosBox`的启动命令，使得`DosBox`启动后转到源文件所在路径；用`cmd`启动`Files\complier.exe`并给定源文件名/源文件路径，输出文件同样在`Files`文件夹下。

