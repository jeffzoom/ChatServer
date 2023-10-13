cmake_minimum_required (VERSION 3.0) #要求cmake最低的版本号
project (main) # 定义当前工程名字

# 设置debug模式，如果没有这一行将不能调试设断点
# set(CMAKE_BUILD_TYPE "Debug")

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 添加头文件路径，相当于makefile里面的-I
# include_directories(${PROJECT_SOURCE_DIR}/include)
# 库文件搜索路径
# link_directories (${PROJECT_SOURCE_DIR}/lib)

# 设置需要编译的源文件列表
set(SRC_LIST ./muduo_server.cpp)

# 设置可执行文件最后总存储的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
# aux_source_directory(. SRC_LIST) 

# 表示生成可执行文件server，由SRC_LIST变量所定义的源文件编译而来
add_executable(server ${SRC_LIST})

# 进入子目录下执行 CMakeLists.txt文件 这里的lib和tests里面都有可编译的代码文件
# add_subdirectory(lib)
# add_subdirectory(tests)

# 表示server这个目标程序，需要连接muduo_net muduo_base pthread这三个库文件
target_link_libraries (server muduo_net muduo_base pthread)
