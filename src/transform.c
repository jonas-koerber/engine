#include "engine/transform.h"

void transform_init(struct Transform* transform) {
    transform->x = 0.0f;
    transform->y = 0.0f;
    transform->z = 0.0f;
    transform->rotationX = 0.0f;
    transform->rotationY = 0.0f;
    transform->rotationZ = 0.0f;
    transform->scaleX = 1.0f;
    transform->scaleY = 1.0f;
    transform->scaleZ = 1.0f;
}