project(qa4mouse)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(libgmouse REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${LIBGMOUSE_INCLUDE_DIRS})

set(QA4Mouse_SRCS QA4Mouse.cpp main.cpp A4Tool.cpp)
set(QA4Mouse_RCCS resource.qrc)

qt4_automoc(${QA4Mouse_SRCS})
qt4_add_resources(QA4Mouse_RCCS_SRCS ${QA4Mouse_RCCS})
add_executable(QA4Mouse A4Device.cpp ${QA4Mouse_SRCS} ${QA4Mouse_RCCS_SRCS})
target_link_libraries(QA4Mouse ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${LIBGMOUSE_LIBRARIES})
install(TARGETS QA4Mouse RUNTIME DESTINATION bin)