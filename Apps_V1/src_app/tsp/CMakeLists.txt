# 头文件路径
include_directories(inc asn inc/http_request inc/tools)


# 源文件路径
aux_source_directory(src SOURCE_DIR)

aux_source_directory(asn ASN_SRC_DIR)

aux_source_directory(src/http_request ASN_SRC_DIR)

aux_source_directory(src/tools ASN_SRC_DIR)

add_executable(tsp ${SOURCE_DIR} ${ASN_SRC_DIR})

target_link_libraries(tsp
                      glib-2.0
                      gio-2.0
                      gobject-2.0
                      pthread
                      sdkapi
                      ssl
                      crypto
		              rt
                      paho-mqtt3cs
                      curl
                      bz2
                      )

install(TARGETS tsp DESTINATION ${CMAKE_INSTALL_PREFIX})