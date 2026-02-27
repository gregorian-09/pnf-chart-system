cmake_minimum_required(VERSION 3.16)

add_executable(pnf_real_world_example real_world_example.cpp)
target_link_libraries(pnf_real_world_example PRIVATE pnf::pnf)
