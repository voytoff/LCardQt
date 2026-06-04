cmake_minimum_required(VERSION 3.20)
project(LCARD LANGUAGES CXX)

set(CMAKE_AUTOMOC ON) # Required for Q_OBJECT
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_testing() # Required to use CTest

add_subdirectory(lib)
add_subdirectory(tests)
