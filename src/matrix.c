#include "engine/matrix.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine/transform.h"

void transform_to_matrix(Mat4* out, const struct Transform* transform) {
    Mat4 scale_matrix;
    matrix_set_identity(&scale_matrix);
    scale_matrix.m[0][0] = transform->scale_x;
    scale_matrix.m[1][1] = transform->scale_y;
    scale_matrix.m[2][2] = transform->scale_z;

    // Add full rotation later
    // For now, just use rotation_z
    Mat4 rotation_matrix;
    matrix_create_rotation_z(&rotation_matrix, transform->rotation_z);

    Mat4 translation_matrix;
    matrix_set_identity(&translation_matrix);
    translation_matrix.m[0][3] = transform->x;
    translation_matrix.m[1][3] = transform->y;
    translation_matrix.m[2][3] = transform->z;

    Mat4 translation_rotation_matrix;
    matrix_multiply(&translation_rotation_matrix, &translation_matrix, &rotation_matrix);
    matrix_multiply(out, &translation_rotation_matrix, &scale_matrix);
}

void matrix_set_identity(Mat4* out) {
    memset(out->m, 0, sizeof(out->m));
    out->m[0][0] = 1.0f;
    out->m[1][1] = 1.0f;
    out->m[2][2] = 1.0f;
    out->m[3][3] = 1.0f;
}

void matrix_create_perspective(Mat4* out, float fov_radians, float aspect_ratio, float near_plane, float far_plane) {
    // Ensure near_plane is not too close to zero
    if (near_plane <= 0.0f) {
        near_plane = 0.001f; // Or some small epsilon
    }
    // Ensure far_plane is greater than near_plane
    if (far_plane <= near_plane) {
        far_plane = near_plane + 1.0f; // Adjust if needed
    }

    // Calculate 'f' from Field of View
    float f = 1.0f / tanf(fov_radians / 2.0f);

    // Start with a matrix of all zeros
    memset(out->m, 0, sizeof(out->m));

    // Set the perspective elements (column-major layout mapping to m[row][col])
    out->m[0][0] = f / aspect_ratio;
    out->m[1][1] = f;
    out->m[2][2] = (far_plane + near_plane) / (near_plane - far_plane);
    out->m[2][3] = (2.0f * far_plane * near_plane) / (near_plane - far_plane);
    out->m[3][2] = -1.0f;
    // m[3][3] remains 0 (crucial for perspective divide using w)
}

void matrix_create_camera(Mat4* out) {
    matrix_set_identity(out);
    out->m[2][3] = -5.0f;
}

void matrix_create_rotation_z(Mat4* out, float angle_radians) {
    matrix_set_identity(out);
    float cos_angle = cosf(angle_radians);
    float sin_angle = sinf(angle_radians);
    out->m[0][0] = cos_angle;
    out->m[0][1] = -sin_angle;
    out->m[1][0] = sin_angle;
    out->m[1][1] = cos_angle;
}

void matrix_multiply(Mat4* out, const Mat4* a, const Mat4* b) {
    Mat4 temp;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp.m[i][j] = a->m[i][0] * b->m[0][j] +
                          a->m[i][1] * b->m[1][j] +
                          a->m[i][2] * b->m[2][j] +
                          a->m[i][3] * b->m[3][j];
        }
    }

    memcpy(out->m, temp.m, sizeof(temp.m));
}