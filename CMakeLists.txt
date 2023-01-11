cmake_minimum_required(VERSION 3.24)
project(label_propagation)

set(CMAKE_CXX_STANDARD 11)

add_executable(label_propagation
        main.cpp
        network.cpp
        node.cpp
        SparseMatrix.cpp
        exceptions.h)
