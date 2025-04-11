#include "engine/transform.h"

void transform_init(struct Transform* transform) {
    transform->x = 0.0f;
    transform->y = 0.0f;
    transform->z = 0.0f;
    transform->rotation_x = 0.0f;
    transform->rotation_y = 0.0f;
    transform->rotation_z = 0.0f;
    transform->scale_x = 1.0f;
    transform->scale_y = 1.0f;
    transform->scale_z = 1.0f;
}