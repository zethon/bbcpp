project(bbcppexamples)
add_executable(bbcpp
    main.cpp
    cxxopts.hpp)

target_link_libraries(bbcpp
    bbcppstatic
)
