#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "myqlwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 public slots:
  void update_frame();

 private slots:
  void on_but_setColorVertex_clicked();

  void on_but_setColorLine_clicked();

  void on_but_setColorBack_clicked();

  void on_pushButton_clicked();

  void on_x_pos_valueChanged(int arg1);

  void on_y_pos_valueChanged(int arg1);

  void on_z_pos_valueChanged(int arg1);

  void on_x_rotate_valueChanged(int arg1);

  void on_y_rotate_valueChanged(int arg1);

  void on_z_rotate_valueChanged(int arg1);

  void on_scale_valueChanged(double arg1);

  void on_size_vertex_valueChanged(int arg1);

  void on_size_line_valueChanged(int arg1);

  void on_but_circle_vertex_clicked();

  void on_but_square_vertex_clicked();

  void on_but_none_vertex_clicked();

  void on_but_solid_line_clicked();

  void on_radioButton_5_clicked();

  void on_but_central_proection_clicked();

  void on_but_paralel_proection_clicked();

  void on_but_jpeg_clicked();

  void on_but_bmp_clicked();

  void on_but_gif_clicked();

 private:
  Ui::MainWindow *ui;
  MyQLWidget *openGLWidget;
  QColor colorVertex = Qt::yellow, colorLine, colorBG;
  QString file;
  QImage frames[50];
  int count = 0;
  QTimer *timer;
  QString selectedFolder;
  void picture_save(QString resolution);
  void save_settings();
  void load_settings();
  void setDefault();
};

#endif  // MAINWINDOW_H
