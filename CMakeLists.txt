project(QA4Tech)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(QA4Tech_SRCS QA4Tech.cpp main.cpp)
qt4_automoc(${QA4Tech_SRCS})
add_executable(QA4Tech ${QA4Tech_SRCS})
target_link_libraries(QA4Tech ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS QA4Tech RUNTIME DESTINATION bin)
