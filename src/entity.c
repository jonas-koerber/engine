#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "engine/entity.h"
#include "engine/mesh.h"
#include "engine/transform.h"

struct Entity* entity_create(struct Mesh* mesh, struct Transform* transform) {
    struct Entity* entity = malloc(sizeof(struct Entity));
    entity->mesh = mesh;
    entity->transform = transform;
    return entity;
}

void entity_destroy(struct Entity* entity) {
    free(entity);
}