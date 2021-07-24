cmake_minimum_required(VERSION 3.16)

project(
  player
  DESCRIPTION "Schönherz Mátrix animation player for mp4"
  HOMEPAGE_URL "kszk.bme.hu"
  LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(LIBMUEB_AS_DLL)

find_package(
  Qt5
  COMPONENTS Widgets MultimediaWidgets
  REQUIRED)

find_package(libmueb 2.0 REQUIRED)

add_executable(
  ${PROJECT_NAME} WIN32
  src/main.cpp src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
  src/videosurface.cpp src/videosurface.h)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::MultimediaWidgets
                          libmueb::muebtransmitter)
