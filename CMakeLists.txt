cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Minesweeper)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6.2 COMPONENTS system window graphics audio REQUIRED)

set(SOURCES main.cpp util.cpp TextureLoader.cpp Game/Game.cpp Game/Tile.cpp "UI Components/Counter.cpp" Animation/Animatable.cpp Animation/PhysicsAnimatable.cpp Animation/Explosion.cpp)

add_executable(Minesweeper ${SOURCES})
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio)