cmake_minimum_required(VERSION 3.16)

project(LCard LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

include_directories("C:/Program Files (x86)/L-Card/ltr/include")
if(WIN32)
  set(LCARD_DIR, "C:/Program Files (x86)/L-Card")
  include_directories("C:/Program Files (x86)/L-Card/ltr/include")
  link_directories("C:/Program Files (x86)/L-Card/ltr/lib/msvc64")
elseif(LINUX)
  set(LCARD_DIR, "/usr/include")
  include_directories("${LCARD_DIR}/ltr/include")
  link_directories("${LCARD_DIR}/ltr/lib")
endif()

add_library(LCard SHARED
  LCard_global.h
  lcard.cpp
  lcard.h
  api11.h api11.cpp
  create.h create.cpp
)

target_link_libraries(LCard PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
)
if(WIN32)
  target_link_libraries(LCard PRIVATE
    ltrapi.lib
    ltr11api.lib
  )
elseif(LINUX)
  target_link_libraries(LCard PRIVATE
    libltrapi.so.1
    libltr11api.so.1
  )
endif()

target_compile_definitions(LCard PRIVATE LCARD_LIBRARY)
