all:
	gcc main.c train.c materials.c -o sdl_app.exe `sdl2-config --cflags --libs`

clean:
	del sdl_app.exe
