cmake_minimum_required(VERSION 3.21)
project(app LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5/lib/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    main.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    include/*.hpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
)
