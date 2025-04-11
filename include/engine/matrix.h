#pragma once

struct Transform;

typedef struct { float m[4][4]; } Mat4;
typedef struct { float x, y, z; } Vec3;

void transform_to_matrix(Mat4* outMatrix, const struct Transform* transform);

void matrix_set_identity(Mat4* matrix);

void matrix_create_perspective(Mat4* out, float fov_radians, float aspect_ratio, float near_plane, float far_plane);

void matrix_create_camera(Mat4* out);

void matrix_create_rotation_z(Mat4* out, float angle_radians);

void matrix_multiply(Mat4* out, const Mat4* a, const Mat4* b);

// some future functions
// void matrix_create_translation(Mat4* out, float tx, float ty, float tz);
// void matrix_create_scale(Mat4* out, float sx, float sy, float sz);
// void matrix_create_rotation_x(Mat4* out, float angle_radians);
// void matrix_create_rotation_y(Mat4* out, float angle_radians);