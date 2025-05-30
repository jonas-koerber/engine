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

# --- OS Detection and Conditional Linker Flags ---
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    # Linker flags for Linux
    LDFLAGS = -lglfw -lGL -lm -ldl -pthread
else ifeq ($(UNAME_S),Darwin)
    # Linker flags for macOS
    LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm -ldl -pthread
else
    # Fallback for other OSes (optional: issue a warning or use a default)
    $(warning "Unsupported OS: $(UNAME_S). Attempting to use Linux LDFLAGS as default.")
    LDFLAGS = -lglfw -lGL -lm -ldl -pthread
endif
# --- End of OS Detection and Conditional Linker Flags ---

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)