此文档用于共同记录clion开发中遇到有关环境的一些问题

1、文件路径设置

编译文件放在`cmake-build-debug`文件夹中，此时使用相对路径`"./(relative path)"`是在`cmake-build-debug`文件夹下寻找文件

需要回到代码文件同目录下的相对路径，因此回到父目录 `"../(relative path)"`
即可调用成功

2、头文件/源文件路径

`#include"\(relative path)"`

是相对于文件所在位置，不是相对于`cmake-build-debug`

很神秘