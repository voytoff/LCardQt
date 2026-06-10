cmake_minimum_required(VERSION 3.16)

project(test1 LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 REQUIRED COMPONENTS
  Core
  Widgets
  Test
  Network
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Core
  Widgets
  Test
  Network
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../lib)

add_executable(test1
  tst_test1.cpp
  dialog.h dialog.cpp dialog.ui

)

if(WIN32)
  include_directories("C:/Program Files (x86)/L-Card/ltr/include")
elseif(LINUX)
  set(test1, "/usr/include")
  include_directories("${LCARD_DIR}/ltr/include")
endif()

add_test(NAME test1 COMMAND test1)
target_link_libraries(test1 PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Test
  Qt${QT_VERSION_MAJOR}::Network
  LCard
)

