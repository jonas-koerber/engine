#include <glad/glad.h>
#include "engine/display.h"
#include <stdio.h>

void display_init(struct Display* display, int width, int height, const char* title) {
    display->width = width;
    display->height = height;
    display->title = title;
    
    // Initialize GLFW if not already done
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return;
    }
    
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    // Set window hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    
    // Create window
    display->window = glfwCreateWindow(mode->width, mode->height, display->title, primaryMonitor, NULL);
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

    glEnable(GL_DEPTH_TEST);
    
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