CC = gcc
# Source files
SRCS = src/main.c src/glad.c src/display.c src/shader.c src/input.c \
       src/file.c src/mesh.c src/entity.c src/transform.c src/matrix.c src/obj.c
# Object files
OBJS = $(SRCS:.c=.o)
# Executable name
EXEC = game_engine

# Compiler flags
CFLAGS = -Wall -Wextra -g -Iinclude 
# Linker flags (ensure you link against GLFW, OpenGL, and math library)
LDFLAGS = -lglfw -lGL -lm -ldl -pthread 

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)