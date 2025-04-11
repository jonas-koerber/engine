#pragma once

struct Transform {
    float x;
    float y;
    float z;
    float rotationX;
    float rotationY;
    float rotationZ;
    float scaleX;
    float scaleY;
    float scaleZ;
};

void transform_init(struct Transform* transform);