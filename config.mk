# os name
UNAME := $(shell uname)

# opengl includes and libs
ifeq (${UNAME}, Darwin)
GL_INCS =
GL_LIBS = -framework OpenGL -framework GLUT
else
GL_INCS =
GL_LIBS = -lGL -lGLEW -lglut
endif

# includes and libs
INCS = ${GL_INCS}
LIBS = ${GL_LIBS}

# flags
CFLAGS = ${INCS} -std=gnu99 -Wall -O3
LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
