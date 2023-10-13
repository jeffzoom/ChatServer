cmake_minimum_required (VERSION 3.0) #要求cmake最低的版本号
project (main) # 定义当前工程名字

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 进入子目录下执行 CMakeLists.txt文件 这里的lib和tests里面都有可编译的代码文件
add_subdirectory(testmuduo)
