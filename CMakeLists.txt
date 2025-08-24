cmake_minimum_required(VERSION 3.10)

project(Automata)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0  
)

FetchContent_MakeAvailable(raylib)

include_directories(include)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi -fsanitize=undefined -fsanitize=address -fsanitize=leak")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(main src/main.cpp src/Plant.cpp src/Enviroment.cpp src/Utils.cpp src/Grid.cpp src/Entity.cpp src/DrawStrategy.cpp src/BoardOperations.cpp src/ConfigManager.cpp)


target_link_libraries(main raylib m pthread X11 GL dl)