default:
	gcc -Wall -Wextra -o maze main.c `pkg-config --cflags --libs sdl2`
