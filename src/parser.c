#include "parser.h"

char *fget_str(FILE *file) {
  char buf[161] = {0};
  char *res = NULL;
  int len = 0;
  int n = 0;
  do {
    n = fscanf(file, "%160[^\n]", buf);
    if (n < 0) {
      if (!res) {
        return NULL;
      }
    } else if (n > 0) {
      int chunk_len = strlen(buf);
      int str_len = len + chunk_len;
      res = realloc(res, str_len + 1);
      memcpy(res + len, buf, chunk_len);
      len = str_len;
    } else {
      n = fscanf(file, "%*c");
      n = 0;
    }
  } while (n > 0);

  if (len > 0) {
    res[len] = '\0';
  } else {
    res = calloc(1, sizeof(char));
  }

  return res;
}

data *init_data() {
  data *new_data = malloc(sizeof(data));
  new_data->countVertex = 0;
  new_data->countFace = 0;
  new_data->matrix = malloc(sizeof(matrix_t));
  new_data->polygons = malloc(sizeof(polygon_t));
  new_data->matrix->rows = 0;
  new_data->matrix->matrix = NULL;
  new_data->polygons->countVertex = 0;
  new_data->polygons->vertexes = NULL;
  new_data->correct = 1;
  new_data->allocFace = 0;
  return new_data;
}

int parser_v(char *str, data *parser_data) {
  double coord[3];
  char buff[255] = {'\0'};
  int pos = 0, index = 0;
  int result = 0;
  for (int i = 2; i < (int)strlen(str); i++) {
    if (str[i] == '-' && pos == 0) {
      buff[pos++] = str[i];
    } else if (strchr("0123456789.,", str[i])) {
      buff[pos++] = str[i];
    } else {
      if (pos != 0 && str[i] == ' ') {
        if (index == 3) {
          result = 1;
        } else {
          buff[pos++] = '\0';
          coord[index++] = atof(buff);
        }
        pos = 0;
      } else if (str[i] != ' ' && str[i] != '\n' && str[i] != (char)13) {
        result = 1;
      }
    }
  }
  if (pos != 0) {
    if (index == 3) {
      result = 1;
    } else {
      buff[pos++] = '\0';
      coord[index++] = atof(buff);
    }
    pos = 0;
  }
  if (result == 0) {
    parser_data->matrix->minMaxX[0] =
        fmin(parser_data->matrix->minMaxX[0], coord[0]);
    parser_data->matrix->minMaxX[1] =
        fmax(parser_data->matrix->minMaxX[1], coord[0]);
    parser_data->matrix->minMaxY[0] =
        fmin(parser_data->matrix->minMaxY[0], coord[1]);
    parser_data->matrix->minMaxY[0] =
        fmax(parser_data->matrix->minMaxY[1], coord[1]);
    parser_data->matrix->minMaxZ[0] =
        fmin(parser_data->matrix->minMaxZ[0], coord[2]);
    parser_data->matrix->minMaxZ[0] =
        fmax(parser_data->matrix->minMaxZ[1], coord[2]);
    if (parser_data->countVertex == 0) {
      parser_data->countVertex = 1;
      parser_data->matrix->rows = 1;
      parser_data->matrix->matrix = (double *)malloc(sizeof(double) * 3);
      parser_data->matrix->matrix[0] = coord[0];
      parser_data->matrix->matrix[1] = coord[1];
      parser_data->matrix->matrix[2] = coord[2];
    } else {
      if (parser_data->countVertex == parser_data->matrix->rows) {
        parser_data->matrix->matrix = (double *)realloc(
            parser_data->matrix->matrix,
            sizeof(double) * (parser_data->countVertex + 1000) * 3);
        parser_data->matrix->rows = parser_data->countVertex + 1000;
      }
      parser_data->matrix->matrix[parser_data->countVertex * 3] = coord[0];
      parser_data->matrix->matrix[parser_data->countVertex * 3 + 1] = coord[1];
      parser_data->matrix->matrix[parser_data->countVertex * 3 + 2] = coord[2];
      parser_data->countVertex++;
    }
  } else {
  }
  return result;
}

int parser_f(char *str, data *parser_data) {
  char buff[255] = {'\0'};
  int pos = 0;
  int result = 0;
  int firstNumber = 1;
  if (parser_data->countFace == 0) {
    parser_data->polygons = malloc(sizeof(polygon_t));
    parser_data->polygons[0].countVertex = 0;
    parser_data->allocFace = 1;
  } else if (parser_data->countFace == parser_data->allocFace) {
    parser_data->polygons =
        realloc(parser_data->polygons,
                sizeof(polygon_t) * (parser_data->allocFace + 1000));
    parser_data->polygons[parser_data->countFace].countVertex = 0;
    for (int j = parser_data->countFace; j < parser_data->countFace + 1000;
         ++j) {
      parser_data->polygons[j].countVertex = 0;
    }
    parser_data->allocFace += 1000;
  }
  for (int i = 2; i < (int)strlen(str); i++) {
    if (strchr("01234567890", str[i]) && firstNumber == 1) {
      buff[pos++] = str[i];
    } else {
      if (pos != 0) {
        buff[pos++] = '\0';
        int number = parser_data->polygons[parser_data->countFace].countVertex;
        if (number == 0) {
          parser_data->polygons[parser_data->countFace].countVertex = 1;
          parser_data->polygons[parser_data->countFace].vertexes =
              malloc(sizeof(int));
          parser_data->polygons[parser_data->countFace].vertexes[number] =
              atoi(buff) - 1;
        } else {
          if (number == 1) {
            parser_data->polygons[parser_data->countFace].vertexes =
                realloc(parser_data->polygons[parser_data->countFace].vertexes,
                        (number + 1) * sizeof(int));
            parser_data->polygons[parser_data->countFace].vertexes[number] =
                atoi(buff) - 1;
            parser_data->polygons[parser_data->countFace].countVertex++;
          } else {
            parser_data->polygons[parser_data->countFace].vertexes =
                realloc(parser_data->polygons[parser_data->countFace].vertexes,
                        (number + 2) * sizeof(int));
            parser_data->polygons[parser_data->countFace].vertexes[number] =
                parser_data->polygons[parser_data->countFace]
                    .vertexes[number - 1];
            parser_data->polygons[parser_data->countFace].countVertex += 2;
            parser_data->polygons[parser_data->countFace].vertexes[number + 1] =
                atoi(buff) - 1;
          }
        }
        pos = 0;
        firstNumber = 2;
      }
      if (str[i] == ' ') {
        firstNumber = 1;
      }
      if (!strchr("01234567890/ ", str[i])) {
        result = 1;
      }
    }
  }
  if (pos != 0) {
    buff[pos++] = '\0';
    int number = parser_data->polygons[parser_data->countFace].countVertex;
    if (number == 0) {
      parser_data->polygons[parser_data->countFace].countVertex = 1;
      parser_data->polygons[parser_data->countFace].vertexes =
          malloc(sizeof(int));
      parser_data->polygons[parser_data->countFace].vertexes[number] =
          atoi(buff) - 1;
    } else {
      parser_data->polygons[parser_data->countFace].vertexes =
          realloc(parser_data->polygons[parser_data->countFace].vertexes,
                  (number + 2) * sizeof(int));
      parser_data->polygons[parser_data->countFace].vertexes[number] =
          parser_data->polygons[parser_data->countFace].vertexes[number - 1];
      parser_data->polygons[parser_data->countFace].countVertex += 2;
      parser_data->polygons[parser_data->countFace].vertexes[number + 1] =
          atoi(buff) - 1;
    }
  }
  parser_data->countFace++;

  return result;
}

data *parser(char *fileName) {
  FILE *fp = fopen(fileName, "r");
  data *result = NULL;
  if (fp) {
    result = init_data();
    char *line = NULL;
    while ((line = fget_str(fp)) != NULL) {
      int cor = 0;
      if (line[0] == 'v' && line[1] == ' ') {
        cor = parser_v(line, result);
      } else if (line[0] == 'f') {
        cor = parser_f(line, result);
      }
      free(line);
      result->correct = cor;
    }
    fclose(fp);
  }
  return result;
}

void free_struct(data *data) {
  if (data) {
    if (data->matrix) {
      free(data->matrix->matrix);
      free(data->matrix);
    }
    if (data->polygons) {
      for (int i = 0; i < data->countFace; ++i) {
        free(data->polygons[i].vertexes);
      }
      free(data->polygons);
    }
    free(data);
  }
}
