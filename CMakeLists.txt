cmake_minimum_required(VERSION 3.28)
project(FAT32FileSystem)
set(CMAKE_CXX_STANDARD 17)
set(ADDRESS_MODEL 32)
set(NODE_TARGET x86)


aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_List)
#set(SRC_List )


add_executable(prog.bin ${SRC_List})
