#pragma once

struct Display {
    GLFWwindow* window;
    int width;
    int height;
    const char* title;
};

void display_init(struct Display* display, int width, int height, const char* title);

void display_destroy(struct Display* display);

int display_should_close(struct Display* display);

void display_update(struct Display* display);