cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_VERSION 10.0.18362.0)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output)

include(cmake/config.cmake)
include(cmake/junuo.cmake)

project(kmshare)

set(sources
    startup.cpp
    
    core/event/abstractevent.h
    core/event/keyboardevent.h
    core/event/keyboardevent.cpp
    core/event/keycodedefs.h
    core/event/mouseevent.h
    core/event/mouseevent.cpp
    core/event/eventqueue.h
    core/event/eventqueue.cpp

    core/hook/hook.h
    core/hook/hook.cpp

    core/network/kmshareserver.h
    core/network/kmshareserver.cpp
    core/network/kmshareclient.h
    core/network/kmshareclient.cpp

    gui/kmsharewindow.h
    gui/kmsharewindow.cpp
    gui/subwidget/serverwidget.h
    gui/subwidget/serverwidget.cpp
    gui/subwidget/clientwidget.h
    gui/subwidget/clientwidget.cpp
    gui/subwidget/devicewidget.h
    gui/subwidget/devicewidget.cpp
)

junuo_auto_moc(generateFiles ${sources})
include_directories(.)
junuo_add_executable(kmshare ${sources})
junuo_use_Qt(kmshare Core Gui Widgets Network)
junuo_add_generate_sources(kmshare ${generateFiles})
