cmake_minimum_required(VERSION 3.13)
project(Agg)

set(CMAKE_CXX_STANDARD 14)
set(Eigen3_DIR "/usr/local/eigenlibrary")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

INCLUDE_DIRECTORIES(/usr/local/Cellar/zeromq/4.3.1_1/include)
LINK_DIRECTORIES(/usr/local/Cellar/zeromq/4.3.1_1/lib)

add_executable(Agg main.cpp matMsg.h matMsg.cpp)

target_link_libraries(Agg zmq)
target_link_libraries (Agg Eigen3::Eigen)
