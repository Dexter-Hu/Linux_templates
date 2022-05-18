cmake_minimum_required(VERSION 3.0.0) #规定了cmake的最低版本号
project(internet_socket_server VERSION 1.0.0)  #规定了工程名和工程版本号

set(CMAKE_CXX_FLAGS "${CAMKE_C_FLAGS}   -Wall -std=c++11 -pthread -fPIC") #-fPIC=生成与位置没关系的代码
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -Wall -std=c++11 -pthread -fPIC") #设置编译选项,-Wall=警告, -std=c++0x=支持C++11 -pthread=线程相关

set(INTERNET_SOCKET_SERVER ${PROJECT_SOURCE_DIR}/internet_socket_server)
set(INTERNET_SOCKET_CLIENT ${PROJECT_SOURCE_DIR}/internet_socket_client)
subdirs(INTERNET_SOCKET_SERVER INTERNET_SOCKET_CLIENT)