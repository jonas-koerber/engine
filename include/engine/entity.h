#pragma once

struct Mesh;

struct Entity {
    struct Mesh* mesh;
};

struct Entity* entity_create(struct Mesh* mesh);
void entity_destroy(struct Entity* entity);