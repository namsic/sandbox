cmake_minimum_required(VERSION 3.27)

project(my_project
    LANGUAGES C
    VERSION 0.0.1
)

# set(CMAKE_INSTALL_RPATH /home/namsic/.local/lib)

option(ENABLE_ARCUS "Enable ARCUS API")

add_executable(app main.c)

if(ENABLE_ARCUS)
    message(STATUS "ENABLE_ARCUS")
    target_compile_definitions(app PRIVATE ENABLE_ARCUS)
endif()

target_include_directories(app PRIVATE
    /home/namsic/.local/include
)
target_link_directories(app PRIVATE
    /home/namsic/.local/lib)
target_link_libraries(app PRIVATE
    memcached
    memcachedutil
)
