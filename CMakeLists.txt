project(qa4mouse)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(BIN_INSTALL_DIR bin)
set(XDG_APPS_INSTALL_DIR share/applications/)
set(ICON_INSTALL_DIR share/icons)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(libgmouse REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${LIBGMOUSE_INCLUDE_DIRS})

set(QA4Mouse_SRCS Profile.cpp QA4Mouse.cpp main.cpp A4Tool.cpp)
set(QA4Mouse_RCCS resource.qrc)

qt4_automoc(${QA4Mouse_SRCS})
qt4_add_resources(QA4Mouse_RCCS_SRCS ${QA4Mouse_RCCS})
add_executable(qa4mouse A4Device.cpp ${QA4Mouse_SRCS} ${QA4Mouse_RCCS_SRCS})
target_link_libraries(qa4mouse ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${LIBGMOUSE_LIBRARIES})

install(TARGETS qa4mouse RUNTIME DESTINATION ${BIN_INSTALL_DIR})
install(PROGRAMS qa4mouse.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/qa4mouse.png DESTINATION ${ICON_INSTALL_DIR})