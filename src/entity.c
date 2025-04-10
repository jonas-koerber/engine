#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "engine/entity.h"
#include "engine/mesh.h"

struct Entity* entity_create(struct Mesh* mesh) {
    struct Entity* entity = malloc(sizeof(struct Entity));
    entity->mesh = mesh;
    return entity;
}