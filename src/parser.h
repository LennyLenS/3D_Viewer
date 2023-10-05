#ifndef PARSER_H
#define PARSER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_t {
  double *matrix;
  int rows;

  double minMaxX[2];
  double minMaxY[2];
  double minMaxZ[2];
} matrix_t;

typedef struct polygon_t {
  int *vertexes;
  int countVertex;
} polygon_t;

typedef struct data {
  int countVertex;
  int countFace;
  int allocFace;
  matrix_t *matrix;
  polygon_t *polygons;
  int correct;
} data;

data *parser(char *fileName);
void free_struct(data *data);
#endif  // PARSER_H
