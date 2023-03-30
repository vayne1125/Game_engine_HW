CXX = g++
CXXFLAGS = $(shell pkg-config --cflags opengl freeglut glesv2 glew glut gl) -Wall
LDFLAGS = $(shell pkg-config --libs opengl freeglut glesv2 glew glut gl)
SOURCES := $(wildcard *.cpp) ./glad/glad.c 

all: main

main: $(SOURCES)

.PHONY: clean
clean:
	rm -f *.o main
