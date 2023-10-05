#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

#include <math.h>

#include "parser.h"

void rotate_y(double angle, struct matrix_t *points);

void rotate_z(double angle, struct matrix_t *points);

void rotate_x(double angle, struct matrix_t *points);

void scale(double coefficient, struct matrix_t *points);

void transmit(double x_coord, double y_coord, double z_coord,
              struct matrix_t *points);
void normalize_size(struct matrix_t *points);
void center(struct matrix_t *points);

#endif  // AFFINETRANSFORM_H