cmake_minimum_required(VERSION 3.2)

PROJECT(feather_convert_caffe)

SET(CMAKE_BUILD_TYPE "Release") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} /O1 /MT /OPT:NOREF")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} /O3 /MT /OPT:NOREF")


#3.head file path，头文件目录
INCLUDE_DIRECTORIES(
.
install/include
)

link_directories(${PROJECT_SOURCE_DIR}/install/lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/..)

ADD_EXECUTABLE(${PROJECT_NAME} feather_convert_caffe.cc caffe.pb.cc)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} libprotobuf )

