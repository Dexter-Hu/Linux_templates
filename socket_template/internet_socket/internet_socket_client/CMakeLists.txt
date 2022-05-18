cmake_minimum_required(VERSION 3.0.0) #规定了cmake的最低版本号
project(internet_socket_client VERSION 1.0.0)  #规定了工程名和工程版本号

set(CMAKE_CXX_FLAGS "${CAMKE_C_FLAGS}   -Wall -std=c++11 -pthread -fPIC") #-fPIC=生成与位置没关系的代码
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -Wall -std=c++11 -pthread -fPIC") #设置编译选项,-Wall=警告, -std=c++0x=支持C++11 -pthread=线程相关的

#头文件路径
set(READLINE_H ${PROJECT_SOURCE_DIR}/../../readline) #readline
set(INTERNET_SOCKET_CLIENT_H .)                      #internet_socket_client
include_directories (${READLINE_H} ${INTERNET_SOCKET_CLIENT_H})

#源文件配置
aux_source_directory(../../readline READLINE_CPP)    #readline
aux_source_directory(. INTERNET_SOCKET_CLIENT_CPP)   #internet_socket_client
set(APP_SOURCE_FILES ${READLINE_CPP} ${INTERNET_SOCKET_CLIENT_CPP})            

add_executable(${PROJECT_NAME} ${APP_SOURCE_FILES}) #生成可执行文件