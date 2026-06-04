cmake_minimum_required(VERSION 3.16)

project(LCard LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_path(LCARD_INCLUDE_DIR lcomp.h PATHS /usr/include /usr/local/include)
find_library(LCARD_LIBRARY lcomp PATHS /usr/lib /usr/local/lib)
include_directories(${LCARD_INCLUDE_DIR})


if(WIN32)
  include_directories("C:/Program Files (x86)/L-Card")
elseif(LINUX)
  include_directories("/usr/include/ltr")
endif()

add_library(LCard SHARED
  LCard_global.h
  lcard.cpp
  lcard.h
  api11.h api11.cpp
)

target_link_libraries(LCard PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  ${LCARD_LIBRARY}
)
target_compile_definitions(LCard PRIVATE LCARD_LIBRARY)
