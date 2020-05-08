cmake_minimum_required(VERSION 3.13)
project(MovieLens_client)

set(CMAKE_CXX_STANDARD 14)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
INCLUDE_DIRECTORIES(/usr/local/Cellar/zeromq/4.3.1_1/include)
LINK_DIRECTORIES(/usr/local/Cellar/zeromq/4.3.1_1/lib)


add_executable(MovieLens_client main.cpp readMovielens.h readMovielens.cpp dpCoupling.h dpCoupling.cpp)
target_link_libraries (MovieLens_client Eigen3::Eigen)
target_link_libraries (MovieLens_client zmq)
