#include "engine/display.h"

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void display_init(struct Display* display, int width, int height, const char* title) {
    display->width = width;
    display->height = height;
    display->title = title;
    
    // Initialize GLFW if not already done
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return;
    }
    
    // Set window hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    display->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (display->window == NULL) {
        printf("Failed to open GLFW window\n");
        glfwTerminate();
        return;
    }
    
    // Make OpenGL context current
    glfwMakeContextCurrent(display->window);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return;
    }
    
    // Set viewport
    glViewport(0, 0, width, height);
}

void display_destroy(struct Display* display) {
    glfwDestroyWindow(display->window);
    glfwTerminate();
}

int display_should_close(struct Display* display) {
    return glfwWindowShouldClose(display->window);
}

void display_update(struct Display* display) {
    glfwSwapBuffers(display->window);
    glfwPollEvents();
}