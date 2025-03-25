cmake_minimum_required(VERSION 3.31)
project("mydemo" LANGUAGES C CXX)

set(APP_NAME ${PROJECT_NAME})
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR amd64)
set(CMAKE_C_COMPILER   gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17) 
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "请别在源码目录编译, 污染工程")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX17)
  message("---- support cpp 17 ----")
elseif(COMPILER_SUPPORTS_CXX14)
  message("---- support cpp 14 ----")
elseif(COMPILER_SUPPORTS_CXX11)
  message("---- support cpp 11 ----")
else()
  message(
    FATAL_ERROR
      "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler."
  )
endif()

##################################################################
# 启用FetchContent模块
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.15.1 
)
FetchContent_MakeAvailable(spdlog)
##################################################################
find_package(Protobuf REQUIRED)
set(PROTO_FILES proto/test.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDS ${PROTO_FILES})
file(
    GLOB_RECURSE OBJ_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
add_executable(
    ${APP_NAME}
    ${OBJ_SRCS}
    # 包含生成的Protobuf源码
    ${PROTO_SRCS}
)
target_include_directories(
    ${APP_NAME}
    PRIVATE
    include
    # proto 编译出来的 c++ 代码在这里
    ${CMAKE_CURRENT_BINARY_DIR}
)
set_target_properties(
    ${APP_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_link_libraries(
    ${APP_NAME}
    PRIVATE
    protobuf::libprotobuf
    spdlog::spdlog
)