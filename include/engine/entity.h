#pragma once

struct Mesh;
struct Transform;

struct Entity {
    struct Mesh* mesh;
    struct Transform* transform;
};

struct Entity* entity_create(struct Mesh* mesh, struct Transform* transform);
void entity_destroy(struct Entity* entity);