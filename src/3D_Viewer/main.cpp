#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("3D Viewer v1.0");
  w.setFixedSize(1300, 700);
  //    w.setGeometry(400, 200, 1300, 800);
  w.show();

  return a.exec();
}
