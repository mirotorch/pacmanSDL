cmake_minimum_required(VERSION 3.0.0)
project(Pacman VERSION 1.0.0)

include(CTest)
enable_testing()
set(CMAKE_C_COMPILER gcc)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
add_executable(Pacman main.c score.h score.c sdl_playground.h sdl_playground.c)
add_library(Input input.h input.c)
add_library(Field field.h field.c)
add_library(PacmanSprite pacman_sprite.h pacman_sprite.c)
add_library(Ghosts ghosts.h ghosts.c)
target_link_libraries(Pacman Input Field PacmanSprite Ghosts SDL2 SDL2_ttf)

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/src
                ${CMAKE_CURRENT_BINARY_DIR}/src)

include(CPack)
