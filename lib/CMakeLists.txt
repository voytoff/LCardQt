cmake_minimum_required(VERSION 3.16)

project(LCard LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS
  Core
  Network
  Concurrent
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Core
  Network
  Concurrent
)

add_library(LCard SHARED
  LCard_global.h
  lctypes.h
  lcard.h lcard.cpp
  api11.h api11.cpp
  crate.h crate.cpp
  ltr11.h ltr11.cpp
  ltrworker.h ltrworker.cpp
  ltrbase.h ltrbase.cpp
)

if(WIN32)
  set(LCARD_DIR, "C:/Program Files (x86)/L-Card")
  target_include_directories(LCard PRIVATE "C:/Program Files (x86)/L-Card/ltr/include")
  target_link_directories(LCard PRIVATE "C:/Program Files (x86)/L-Card/ltr/lib/msvc64")
elseif(LINUX)
  set(LCARD_DIR, "/usr/include")
  target_include_directories(LCard PRIVATE "/usr/include/ltr/include")
  target_link_directories(LCard PRIVATE "/usr/lib")
endif()

target_link_libraries(LCard PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network # Ubuntu/Debian: Run sudo apt install qt6-network-dev
  Qt${QT_VERSION_MAJOR}::Concurrent
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
