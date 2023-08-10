cmake_minimum_required (VERSION  3.8.2)

project(app)

#设置安装目录
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out")
set(CMAKE_CROSS_PATH " /home/yang/tbox/ag35lavd/ql-ol-sdk")
message("[install path is ${CMAKE_INSTALL_PREFIX}]")
message("[cross root path is ${CMAKE_CROSS_PATH}]")

SET(CMAKE_BUILD_TYPE "Debug")



#设置编译器
set(CMAKE_C_COMPILER   "arm-oe-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "arm-oe-linux-gnueabi-g++")

set(CMAKE_CXX_FLAGS "-march=armv7-a -marm -mfpu=neon -mfloat-abi=hard -std=c++11 -D_GNU_SOURCE -fpermissive -O0 -Wall -g2 -ggdb" )
set(CMAKE_C_FLAGS "-march=armv7-a -marm -mfpu=neon -mfloat-abi=hard -O0 -Wall -g2 -ggdb" )


add_compile_options(-Wno-cpp -Wno-deprecated-declarations -Wno-pointer-arith)


#设置头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/include/
                    ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/include/c++/6.4.0/
                    ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/include/c++/6.4.0/arm-oe-linux-gnueabi/
                    ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/include/glib-2.0/
                    ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/include/gio-unix-2.0/
                    ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/lib/glib-2.0/include/)

#设置库文件路径
link_directories(${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/lib/
                 ${CMAKE_CROSS_PATH}/ql-ol-crosstool/sysroots/armv7ahf-neon-oe-linux-gnueabi/usr/lib/
                 ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
                    
message("*******${LD_LIBRARY_PATH} source:${CMAKE_SOURCE_DIR}")

add_subdirectory(src_app/tsp/)
#add_subdirectory(src_app/test/)




