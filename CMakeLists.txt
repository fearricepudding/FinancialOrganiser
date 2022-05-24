cmake_minimum_required(VERSION 3.5)

project(FinancialOrganiser LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(jsoncpp REQUIRED)

include_directories(
  /usr/include
  /usr/local/include
  /usr/include/qt/QtWidgets
  ${QT5_INCLUDE_DIR}
  ${JSONCPP_INCLUDE_DIR}
  "D:/vcpkg/installed/x64-windows/include"
)

add_executable(FinancialOrganiser
  ui/mainwindow.ui
  ui/importstatement.ui
  ui/mystatements.ui
  ui/newbill.ui
  ui/selectbill.ui

  src/main.cpp
  src/mainwindow.cpp
  src/importstatement.cpp
  src/database.cpp
  src/mystatements.cpp
  src/newbill.cpp
  src/selectbill.cpp
  src/statement.cpp
  src/helpers.cpp

  src/mainwindow.h
  src/importstatement.h
  src/database.h
  src/mystatements.h
  src/debug.h
  src/cppcsv.h
  src/newbill.h
  src/selectbill.h
  src/statement.h
  src/helpers.h
)

target_link_libraries(FinancialOrganiser PRIVATE  jsoncpp_lib jsoncpp_object Qt5::Widgets)
