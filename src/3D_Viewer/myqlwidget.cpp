#include "myqlwidget.h"

#include <math.h>

#include <QFloat16>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QtGui>
extern "C" {
#include "../AffineTransform.h"
#include "../parser.h"
}

MyQLWidget::MyQLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void MyQLWidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glLineStipple(1, 0xFFF0);  // Маска пунктирной линии
}

void MyQLWidget::resizeGL(int nWidth, int nHeight) {
  glViewport(0, 0, nWidth, nHeight);
}

void MyQLWidget::paintGL() {
  glClearColor(colorBG.redF(), colorBG.greenF(), colorBG.blueF(),
               colorBG.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projectionType == 0) {
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100);
  } else {
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -3);
  if (vertexType != 0) {
    drawPointView();
  } else {
    drawSolidView();
  }
}

void MyQLWidget::openfile(char *filename) {
  free_struct(parser_data);
  parser_data = parser(filename);
  center(parser_data->matrix);
  normalize_size(parser_data->matrix);
}

void MyQLWidget::drawPointView() {
  if (parser_data != nullptr) {
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    if (vertexType == 1) {
      glDisable(GL_POINT);
      glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    } else {
      glDisable(GL_POINT_SMOOTH);
      glEnable(GL_POINT);
    }
    glColor4f(colorVertex.redF(), colorVertex.greenF(), colorVertex.blueF(),
              colorVertex.alphaF());
    glPointSize(vertexWidth);

    glVertexPointer(3, GL_DOUBLE, 0, parser_data->matrix->matrix);
    glEnableClientState(GL_VERTEX_ARRAY);

    for (int i = 0; i < parser_data->countFace; i++) {
      glDrawElements(GL_POINTS, parser_data->polygons[i].countVertex,
                     GL_UNSIGNED_INT, parser_data->polygons[i].vertexes);
    }

    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_POINT);
  }
}

void MyQLWidget::drawSolidView() {
  if (parser_data != nullptr) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (lineType == 1) glEnable(GL_LINE_STIPPLE);
    glColor4f(colorLine.redF(), colorLine.greenF(), colorLine.blueF(),
              colorLine.alphaF());
    glLineWidth(lineWidth);

    glVertexPointer(3, GL_DOUBLE, 0, parser_data->matrix->matrix);
    glEnableClientState(GL_VERTEX_ARRAY);

    for (int i = 0; i < parser_data->countFace; i++) {
      glDrawElements(GL_LINES, parser_data->polygons[i].countVertex,
                     GL_UNSIGNED_INT, parser_data->polygons[i].vertexes);
    }

    glDisable(GL_LINE_STIPPLE);
  }
}

void MyQLWidget::cleanParameters() {
  xPos = 0;
  yPos = 0;
  zPos = 0;
  xRotate = 0;
  yRotate = 0;
  zRotate = 0;
  scale = 1;
}
MyQLWidget::~MyQLWidget() {}
