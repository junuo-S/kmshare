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

    core/hook/hook.h
    core/hook/hook.cpp
)

junuo_auto_moc(generateFiles ${sources})

junuo_add_executable(kmshare WIN32 ${sources})
junuo_use_Qt(kmshare Core Gui Widgets)
junuo_add_generate_sources(kmshare ${generateFiles})
