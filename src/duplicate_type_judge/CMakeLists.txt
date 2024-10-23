# 查找当前目录下的源文件
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# 生成可执行文件
add_executable(duplicate_type_judge ${SOURCES})