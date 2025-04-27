cmake_minimum_required(VERSION 3.30)
project(cpp_ca3)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_ca3 main.cpp
        Crawler.cpp
        Crawler.h
        Board.cpp
        Board.h
        Bug.cpp
        Bug.h
        Hopper.cpp
        Hopper.h
        SFMLBoard.cpp
        SFMLBoard.h
        Mantis.cpp
        Mantis.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.2/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics network REQUIRED)

include_directories(C:/SFML-2.6.2/include)
target_link_libraries(cpp_ca3 sfml-system sfml-window sfml-graphics)
