all:
	gcc -g -c main.c -O2  -I ../external/cimgui/ -I ../external/cimgui/generator/output/ -I ../external/quickjs/
	gcc -g -c jsrender.c -O2 -I ../external/cimgui/ -I ../external/cimgui/generator/output/ -I ../external/quickjs/
	g++  -g main.o jsrender.o -O2 -D _GNU_SOURCE -rdynamic ./libquickjs.a libcimgui_glfw.a -lm -ldl -lpthread  -lGL -lglfw -Wl,-rpath,'.'


