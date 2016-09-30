set(SOURCE_FILES 
    BBDocument.cpp
)

set(HEADER_FILES
    BBDocument.h
)

add_library(bbcppshared SHARED ${SOURCE_FILES})

set_target_properties(bbcppshared PROPERTIES OUTPUT_NAME "bbcpp")
set_target_properties(bbcppshared PROPERTIES VERSION 0.1 SOVERSION 1)

install(TARGETS bbcppshared ARCHIVE DESTINATION ${LIB_INSTALL_DIR} LIBRARY DESTINATION ${LIB_INSTALL_DIR})

if (WIN32)
    add_library(bbcppstatic STATIC ${libmdcpp_SRSC})
    set_target_properties(bbcppstatic PROPERTIES OUTPUT_NAME "bbcpp")
    install(TARGETS bbcppstatic ARCHIVE DESTINATION ${LIB_INSTALL_DIR} LIBRARY DESTINATION ${LIB_INSTALL_DIR})
endif (WIN32)

install(FILES ${HEADER_FILES} DESTINATION include)
