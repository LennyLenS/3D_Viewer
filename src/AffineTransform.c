#include "AffineTransform.h"

void rotate_y(double angle, struct matrix_t *points) {
  for (int i = 0; i < points->rows; i++) {
    double x_buff = points->matrix[i * 3], z_buff = points->matrix[i * 3 + 2];

    points->matrix[i * 3] = x_buff * cos(angle) + z_buff * sin(angle);
    points->matrix[i * 3 + 2] =
        x_buff * sin(angle) * (-1) + z_buff * cos(angle);
  }
}

void rotate_z(double angle, struct matrix_t *points) {
  for (int i = 0; i < points->rows; i++) {
    double x_buff = points->matrix[i * 3], y_buff = points->matrix[i * 3 + 1];

    points->matrix[i * 3] = x_buff * cos(angle) - y_buff * sin(angle);
    points->matrix[i * 3 + 1] = x_buff * sin(angle) + y_buff * cos(angle);
  }
}

void rotate_x(double angle, struct matrix_t *points) {
  for (int i = 0; i < points->rows; i++) {
    double z_buff = points->matrix[i * 3 + 2],
           y_buff = points->matrix[i * 3 + 1];

    points->matrix[i * 3 + 1] = y_buff * cos(angle) - z_buff * sin(angle);
    points->matrix[i * 3 + 2] = y_buff * sin(angle) + z_buff * cos(angle);
  }
}

void scale(double coefficient, struct matrix_t *points) {
  for (int i = 0; i < points->rows; i++) {
    points->matrix[i * 3] *= coefficient;
    points->matrix[i * 3 + 1] *= coefficient;
    points->matrix[i * 3 + 2] *= coefficient;
  }
}

void transmit(double x_coord, double y_coord, double z_coord,
              struct matrix_t *points) {
  for (int i = 0; i < points->rows; i++) {
    points->matrix[i * 3] += x_coord;
    points->matrix[i * 3 + 1] += y_coord;
    points->matrix[i * 3 + 2] += z_coord;
  }
}

void center(struct matrix_t *points) {
  transmit(-(points->minMaxX[0] + points->minMaxX[1]) / 2,
           -(points->minMaxY[0] + points->minMaxY[1]) / 2,
           -(points->minMaxZ[0] + points->minMaxZ[1]) / 2, points);
}

void normalize_size(struct matrix_t *points) {
  double scope = 1;
  scope = fabs(points->minMaxX[0]);
  if (fabs(points->minMaxX[1]) > scope) scope = points->minMaxX[1];
  if (fabs(points->minMaxY[0]) > scope) scope = points->minMaxY[0];
  if (fabs(points->minMaxY[1]) > scope) scope = points->minMaxY[1];
  if (fabs(points->minMaxZ[0]) > scope) scope = points->minMaxZ[0];
  if (fabs(points->minMaxZ[1]) > scope) scope = points->minMaxZ[1];
  scale(2.5 / scope, points);
}
