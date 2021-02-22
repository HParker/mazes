default:
	gcc -o maze main.c `pkg-config --cflags --libs sdl2`
