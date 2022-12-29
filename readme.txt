Requirements:
-Cmake (3.0.0 or higher)
-SDL2

How to use SDL2 with cmake:
https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/



Input:
1 -- wall
0 -- free space with a point
2 -- tablet
3 -- ghosts respawn (number of ghosts is exactly the number of this blocks)
4 -- gate for ghosts, must be just one. 
Next block after the gate can't be a wall (in this case the program will send you corresponding message).
5 -- starting position of pacman.

I strongly recommend making the input map the same height and width. The vertical "tail" will be ignored, 
the horizontal one will be filled by walls.



Launching:
Make sure that src folder is in the same place as the program. It copies to the build folder
automatically (see CMakeLists.txt). Use terminal/command line to launch, enter the input path as parameter. 




Short description:
main -- game loop, memory management
sdl_playground -- window and renderer initialising & destroying 
input -- input handling, creation of temporary int field
field -- initialising & drawing game field, counting of points left on the field
pacman_sprite -- pacman initialising, movement etc
ghosts -- ghosts movement, behavior etc
score -- score and lives display