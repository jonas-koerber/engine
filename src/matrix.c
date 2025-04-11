#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "engine/matrix.h"
#include "engine/transform.h"

void transform_to_matrix(Mat4* outMatrix, const struct Transform* transform) {
    Mat4 scaleMatrix;
    matrix_set_identity(&scaleMatrix);
    scaleMatrix.m[0][0] = transform->scaleX;
    scaleMatrix.m[1][1] = transform->scaleY;
    scaleMatrix.m[2][2] = transform->scaleZ;

    // Add full rotation later
    // For now, just use rotationZ
    Mat4 rotationMatrix;
    matrix_create_rotation_z(&rotationMatrix, transform->rotationZ);

    Mat4 translationMatrix;
    matrix_set_identity(&translationMatrix);
    translationMatrix.m[0][3] = transform->x;
    translationMatrix.m[1][3] = transform->y;
    translationMatrix.m[2][3] = transform->z;

    Mat4 translationRotationMatrix;
    matrix_multiply(&translationRotationMatrix, &translationMatrix, &rotationMatrix);
    matrix_multiply(outMatrix, &translationRotationMatrix, &scaleMatrix);
}

void matrix_set_identity(Mat4* matrix) {
    memset(matrix->m, 0, sizeof(matrix->m));
    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
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