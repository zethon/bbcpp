project(bbcpplib)

set(SOURCE_FILES 
    BBDocument.cpp
)

set(HEADER_FILES
    bbcpputils.h
    BBDocument.h
)

add_library(bbcppstatic STATIC 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

install(FILES ${HEADER_FILES} DESTINATION include)

if (BUILD_SHARED_LIBS)
    add_library(bbcppshared SHARED 
        ${SOURCE_FILES}
        ${HEADER_FILES})
        
    set_target_properties(bbcppshared PROPERTIES OUTPUT_NAME "bbcpp")
    set_target_properties(bbcppshared PROPERTIES VERSION 0.1 SOVERSION 1)
    install(TARGETS bbcppshared 
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} 
        LIBRARY DESTINATION ${LIB_INSTALL_DIR})
endif()

