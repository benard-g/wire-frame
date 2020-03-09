cmake_minimum_required(VERSION 3.10)

project(wire-frame)

#
# CONFIG
#
set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_CXX_FLAGS "/W4 /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else ()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Weffc++")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Ofast")
endif ()

#
# DEPENDENCIES
#
find_package(SFML COMPONENTS system window graphics audio REQUIRED)

#
# Build
#
add_executable(
    ${PROJECT_NAME}

    # Main
    src/main.cpp
    include/Config.hpp src/Config.cpp

    # Core
    include/wf/core/Orientation.hpp
    include/wf/core/Position.hpp
    include/wf/core/Line.hpp
    include/wf/core/EventManager.hpp

    # Graphics
    include/wf/gfx/Window.hpp src/gfx/Window.cpp
    include/wf/gfx/event/WindowClosed.hpp
    include/wf/gfx/event/WindowKeyPressed.hpp

    # 3D
    include/wf/space/shape/IShape.hpp
#    include/wf/space/shape/Cube.hpp src/space/shape/Cube.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE include
)

target_link_libraries(
    ${PROJECT_NAME}
    # SFML
    PRIVATE sfml-system sfml-window sfml-graphics sfml-audio
)
