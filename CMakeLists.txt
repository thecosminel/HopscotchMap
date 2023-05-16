cmake_minimum_required(VERSION 3.25)
project(Lab4_DSA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab4_DSA
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        Map.cpp
        Map.h
        MapIterator.cpp
        MapIterator.h
        ShortTest.cpp
        ShortTest.h)
