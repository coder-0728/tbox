# 头文件路径
include_directories(inc asn)


# 源文件路径
aux_source_directory(src SOURCE_DIR)

aux_source_directory(asn ASN_SRC_DIR)

add_executable(testapp ${SOURCE_DIR} ${ASN_SRC_DIR})

target_link_libraries(testapp ssl crypto)

install(TARGETS testapp DESTINATION ${CMAKE_INSTALL_PREFIX})