project(Wraith)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(WRAITH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(Wraith)

find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGl REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDES})
include_directories(${WRAITH_INCLUDE_DIR})

# Used to find GStreamer.
include(FindPkgConfig)
pkg_check_modules(GStreamer REQUIRED gstreamer-0.10)
pkg_check_modules(GStreamer_base REQUIRED gstreamer-plugins-base-0.10 gstreamer-base-0.10)
pkg_check_modules(GStreamer_fft REQUIRED gstreamer-fft-0.10)
include_directories(${GStreamer_INCLUDE_DIRS})

# Used to find SQLite
#find_package(Sqlite REQUIRED)
#include_directories(${SQLITE_INCLUDE_DIR})

# Used to find TagLib.
#find_package(Taglib REQUIRED)
#include_directories(${TAGLIB_CFLAGS})
#find_package(Taglib-Extras REQUIRED)
#include_directories(${TAGLIB-EXTRAS_CFLAGS})

# Used to find Boost.
set(Boost_USE_SHARED_LIBS ON)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost COMPONENTS unit_test_framework signals thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# ${SQLITE_DEFINITIONS}
add_definitions(${QT_DEFINITIONS})

add_subdirectory(src)
# Include the directory with the test programs.
add_subdirectory(tests)

set(libwraith_SRCS main.cpp)

# File that actually runs wraith.
add_executable(wraith ${libwraith_SRCS})
target_link_libraries(wraith wraithcore wraithgui ${QT_LIBRARIES})
