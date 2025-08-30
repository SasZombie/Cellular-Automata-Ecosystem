cmake_minimum_required(VERSION 3.10)

project(Automata)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(main src/main.cpp src/Plant.cpp src/Enviroment.cpp src/Utils.cpp src/Grid.cpp src/Entity.cpp src/DrawStrategy.cpp src/BoardOperations.cpp src/ConfigManager.cpp)

if(MSVC)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /W4 /permissive-")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

target_link_libraries(main raylib)

else()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -fsanitize=undefined -fsanitize=address -fsanitize=leak")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

target_link_libraries(main raylib m pthread X11 GL dl)

endif()
