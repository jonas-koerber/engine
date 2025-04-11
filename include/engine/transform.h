#pragma once

struct Transform {
    float x;
    float y;
    float z;
    float rotation_x;
    float rotation_y;
    float rotation_z;
    float scale_x;
    float scale_y;
    float scale_z;
};

void transform_init(struct Transform* transform);