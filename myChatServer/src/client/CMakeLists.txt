# 定义了一个SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(. SRC_LIST) 
# .就是把当前路径下client路径下所有源文件的名字都放在SRC_LIST这个变量里面

# 根据client目录下所有源文件的名字来生成可执行文件ChatClient，可执行文件的名字就是ChatClient
add_executable(ChatClient ${SRC_LIST})
# 指定可执行文件链接时需要依赖的库文件
target_link_libraries(ChatClient pthread)