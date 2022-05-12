#! /bin/sh
gcc -o t6 -I/usr/include/SDL2 t6.c -lSDL_image -lSDL2  -lpthread 
gcc -o server_white server_white.c -lpthread 
gcc -o launcher -I/usr/include/SDL2 launcher.c -lSDL_image -lSDL2  -lpthread -lSDL_ttf -std=c99 `sdl-config --libs` -lSDL_ttf
