#ifndef MYQLWIDGET_H
#define MYQLWIDGET_H

#include <QDebug>
#include <QFloat16>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPoint>
#include <QTimer>

extern "C" {
#include "../parser.h"
}

const double K = 0.01;
const int HEIGHT = 800;
const int WIDHT = 600;
class MyQLWidget : public QOpenGLWidget {
  Q_OBJECT

 private:
 private slots:

 protected:
  void initializeGL();
  void resizeGL(int nWidth, int nHeight);
  void paintGL();

 public:
  MyQLWidget(QWidget *parent = 0);
  void openfile(char *filename);
  ~MyQLWidget();

  int lineType = 0;
  int vertexType = 0;
  int lineWidth = 1;
  int vertexWidth = 1;
  int projectionType = 0;
  int xRotate = 0, yRotate = 0, zRotate = 0;
  int xPos = 0, yPos = 0, zPos = 0;
  double scale = 1;
  QColor colorVertex = Qt::white, colorLine = Qt::white, colorBG = Qt::black;

  struct data *parser_data = nullptr;
  void drawPointView();
  void drawSolidView();
  void cleanParameters();
};

#endif  // MYQLWIDGET_H
