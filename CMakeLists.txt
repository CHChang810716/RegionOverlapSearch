cmake_minimum_required(VERSION 3.0.0)
project(RegionOverlapSearch)

add_executable(super_enhancer_cluster example/super_enhancer_cluster.cpp)
target_include_directories(super_enhancer_cluster PRIVATE include)
install(TARGETS super_enhancer_cluster DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)