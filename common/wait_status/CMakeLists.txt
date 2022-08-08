cmake_minimum_required(VERSION 3.0.0) #规定了cmake的最低版本号
project(wait_status VERSION 1.0.0)  #规定了工程名和工程版本号

set(CMAKE_C_FLAGS   "-Wall -pthread -fPIC -lrt ${CMAKE_C_FLAGS}")   #-fPIC=生成与位置没关系的代码,-lrt=posix相关
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -pthread -fPIC ${CMAKE_CXX_FLAGS}") #设置编译选项,-Wall=警告, -std=c++11支持C++11 -pthread=线程相关的
message(${CMAKE_C_FLAGS})
message(${CMAKE_CXX_FLAGS})

#头文件路径
# set(READLINE_H ${PROJECT_SOURCE_DIR}/../../readline) #readline
# set(INTERNET_SOCKET_SERVER_H .)                      #internet_socket_server
# include_directories (wait_status.h)

#源文件配置
# aux_source_directory(../../readline READLINE_CPP)    #readline
# aux_source_directory(. POSIX_SHM)   
set(APP_SOURCE_FILES wait_status.cpp)            

# add_executable(${PROJECT_NAME} ${APP_SOURCE_FILES}) #生成可执行文件
add_library(wait_status SHARED ${APP_SOURCE_FILES}) #生成库
target_link_libraries(${PROJECT_NAME}) #增加链接选项

install(TARGETS ${PROJECT_NAME} #安装库文件
  ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../lib
  LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../lib
  RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin
)

install(FILES wait_status.h 
  DESTINATION  ${CMAKE_CURRENT_LIST_DIR}/../../include
)