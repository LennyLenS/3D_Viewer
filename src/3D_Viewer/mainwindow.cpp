#include "mainwindow.h"

#include <qgifimage.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QPixmap>
#include <QSettings>
#include <QTime>

#include "myqlwidget.h"
#include "ui_mainwindow.h"
extern "C" {
#include "../AffineTransform.h"
#include "../parser.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  openGLWidget = new MyQLWidget(this);
  openGLWidget->move(500, 0);
  openGLWidget->resize(HEIGHT, WIDHT);
  openGLWidget->show();
  load_settings();
  ui->bg_color->setStyleSheet("background-color: " +
                              openGLWidget->colorBG.name());
  ui->vertex_color->setStyleSheet("background-color: " +
                                  openGLWidget->colorVertex.name());
  ui->line_color->setStyleSheet("background-color: " +
                                openGLWidget->colorLine.name());
}

MainWindow::~MainWindow() {
  delete ui;
  save_settings();
  free_struct(openGLWidget->parser_data);
}

void MainWindow::on_but_setColorVertex_clicked() {
  openGLWidget->colorVertex =
      QColorDialog::getColor(openGLWidget->colorVertex, this);
  ui->vertex_color->setStyleSheet("background-color: " +
                                  openGLWidget->colorVertex.name());
  openGLWidget->update();
}

void MainWindow::on_but_setColorLine_clicked() {
  openGLWidget->colorLine =
      QColorDialog::getColor(openGLWidget->colorLine, this);
  ui->line_color->setStyleSheet("background-color: " +
                                openGLWidget->colorLine.name());
  openGLWidget->update();
}

void MainWindow::on_but_setColorBack_clicked() {
  openGLWidget->colorBG = QColorDialog::getColor(openGLWidget->colorBG, this);
  ui->bg_color->setStyleSheet("background-color: " +
                              openGLWidget->colorBG.name());
  openGLWidget->update();
}
void MainWindow::setDefault() {
  ui->x_pos->setValue(0);
  ui->y_pos->setValue(0);
  ui->z_pos->setValue(0);
  ui->z_rotate->setValue(0);
  ui->y_rotate->setValue(0);
  ui->x_rotate->setValue(0);
  ui->scale->setValue(1);
}
void MainWindow::on_pushButton_clicked() {
  file = QFileDialog::getOpenFileName(this, tr("Open Image"), "../",
                                      tr("Image Files (*.obj)"));
  QFileInfo check_file(file);
  if (check_file.exists() && check_file.isFile()) {
    openGLWidget->openfile(file.toLocal8Bit().data());
    if (openGLWidget->parser_data->correct == 0) {
      openGLWidget->cleanParameters();
      setDefault();
      ui->model_info->setText(
          file + " Vertexes: " +
          QString::number(openGLWidget->parser_data->countVertex) +
          " Faces: " + QString::number(openGLWidget->parser_data->countFace));
      openGLWidget->update();
    } else {
      ui->model_info->setText("Uncorrect file");
    }
  } else {
    ui->model_info->setText("Uncorrect file path");
  }
}

// movement
void MainWindow::on_x_pos_valueChanged(int arg1) {
  if (openGLWidget->parser_data != nullptr) {
    int delta = arg1 - openGLWidget->xPos;
    transmit(delta * K, 0, 0, openGLWidget->parser_data->matrix);
    openGLWidget->xPos = arg1;
    openGLWidget->update();
  }
}

void MainWindow::on_y_pos_valueChanged(int arg1) {
  if (openGLWidget->parser_data != nullptr) {
    int delta = arg1 - openGLWidget->yPos;
    transmit(0, delta * K, 0, openGLWidget->parser_data->matrix);
    openGLWidget->yPos = arg1;
    openGLWidget->update();
  }
}

void MainWindow::on_z_pos_valueChanged(int arg1) {
  if (openGLWidget->parser_data != nullptr) {
    int delta = arg1 - openGLWidget->zPos;
    transmit(0, 0, delta * K, openGLWidget->parser_data->matrix);
    openGLWidget->zPos = arg1;
    openGLWidget->update();
  }
}

void MainWindow::on_x_rotate_valueChanged(int arg1) {
  if (openGLWidget->parser_data != nullptr) {
    int delta = arg1 - openGLWidget->xRotate;
    rotate_x(delta * K * 5, openGLWidget->parser_data->matrix);
    openGLWidget->xRotate = arg1;
    openGLWidget->update();
  }
}

void MainWindow::on_y_rotate_valueChanged(int arg1) {
  if (openGLWidget->parser_data != nullptr) {
    int delta = arg1 - openGLWidget->yRotate;
    rotate_y(delta * K * 5, openGLWidget->parser_data->matrix);
    openGLWidget->yRotate = arg1;
    openGLWidget->update();
  }
}

void MainWindow::on_z_rotate_valueChanged(int arg1) {
  if (openGLWidget->parser_data != nullptr) {
    int delta = arg1 - openGLWidget->zRotate;
    rotate_z(delta * K * 5, openGLWidget->parser_data->matrix);
    openGLWidget->zRotate = arg1;
    openGLWidget->update();
  }
}

void MainWindow::on_scale_valueChanged(double arg1) {
  if (openGLWidget->parser_data != nullptr) {
    if (arg1 != 0) {
      scale(1 / K / openGLWidget->scale, openGLWidget->parser_data->matrix);
      scale(arg1 * K, openGLWidget->parser_data->matrix);
      openGLWidget->scale = arg1;
      openGLWidget->update();
    }
  }
}

void MainWindow::on_size_vertex_valueChanged(int arg1) {
  openGLWidget->vertexWidth = arg1;
  openGLWidget->update();
}

void MainWindow::on_size_line_valueChanged(int arg1) {
  openGLWidget->lineWidth = arg1;
  openGLWidget->update();
}

void MainWindow::on_but_circle_vertex_clicked() {
  openGLWidget->vertexType = 1;
  openGLWidget->update();
}

void MainWindow::on_but_square_vertex_clicked() {
  openGLWidget->vertexType = 2;
  openGLWidget->update();
}

void MainWindow::on_but_none_vertex_clicked() {
  openGLWidget->vertexType = 0;
  openGLWidget->update();
}

void MainWindow::on_but_solid_line_clicked() {
  openGLWidget->lineType = 0;
  openGLWidget->update();
}

void MainWindow::on_radioButton_5_clicked() {
  openGLWidget->lineType = 1;
  openGLWidget->update();
}

void MainWindow::on_but_central_proection_clicked() {
  openGLWidget->projectionType = 0;
  openGLWidget->update();
}

void MainWindow::on_but_paralel_proection_clicked() {
  openGLWidget->projectionType = 1;
  openGLWidget->update();
}

void MainWindow::on_but_jpeg_clicked() { picture_save(".jpeg"); }

void MainWindow::on_but_bmp_clicked() { picture_save(".bmp"); }

void MainWindow::picture_save(QString resolution) {
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::Directory);

  dialog.setWindowTitle("Выберите папку");

  if (dialog.exec()) {
    selectedFolder = dialog.selectedFiles().at(0);
    QImage img =
        QImage(openGLWidget->size(), QImage::Format_ARGB32_Premultiplied);
    glReadPixels(500, 100, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    img = img.rgbSwapped();
    img = img.mirrored();
    img.save(selectedFolder + "/Screenshot-" +
             QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss") +
             resolution);
  }
}

void MainWindow::update_frame() {
  QPixmap pixmap = openGLWidget->grab();
  QImage image = pixmap.toImage();
  image = image.scaled(640, 480);
  frames[count] = (image);
  count++;
  if (count == 50) {
    timer->stop();
    QGifImage gif(QSize(640, 480));
    gif.setDefaultDelay(100);
    for (int i = 0; i < 50; i++) gif.addFrame(frames[i]);
    gif.save(selectedFolder + "/screencast_" +
             QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss") +
             ".gif");
    count = 0;
  }
}

void MainWindow::on_but_gif_clicked() {
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::Directory);

  dialog.setWindowTitle("Выберите папку");

  if (dialog.exec()) {
    selectedFolder = dialog.selectedFiles().at(0);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_frame()));
    timer->start(100);
  }
}

void MainWindow::load_settings() {
  QSettings settings("SCHOOL21", "3D_Viewer");

  QVariant buff = settings.value("lineWidth");
  if (buff.isValid()) {
    openGLWidget->lineWidth = buff.toInt();
    ui->size_line->setValue(openGLWidget->lineWidth);
  }

  buff = settings.value("vertexWidth");
  if (buff.isValid()) {
    openGLWidget->vertexWidth = buff.toInt();
    ui->size_vertex->setValue(openGLWidget->vertexWidth);
  }

  buff = settings.value("projectionType");
  if (buff.isValid()) {
    openGLWidget->projectionType = buff.toInt();
    if (openGLWidget->projectionType == 1) {
      ui->but_paralel_proection->setChecked(true);
    }
  }

  buff = settings.value("lineType");
  if (buff.isValid()) {
    openGLWidget->lineType = buff.toInt();
    if (openGLWidget->lineType == 1) {
      ui->radioButton_5->setChecked(true);
    }
  }

  buff = settings.value("vertexType");
  if (buff.isValid()) {
    openGLWidget->vertexType = buff.toInt();
    if (openGLWidget->vertexType == 1) {
      ui->but_circle_vertex->setChecked(true);
    } else if (openGLWidget->vertexType == 2) {
      ui->but_square_vertex->setChecked(true);
    }
  }

  QVariant buff2 = settings.value("colorBG");
  if (buff2.isValid()) {
    openGLWidget->colorBG = buff2.toString();
  }

  buff2 = settings.value("colorVertex");
  if (buff2.isValid()) {
    openGLWidget->colorVertex = buff2.toString();
  }

  buff2 = settings.value("colorLine");
  if (buff2.isValid()) {
    openGLWidget->colorLine = buff2.toString();
  }
}

void MainWindow::save_settings() {
  QSettings settings("SCHOOL21", "3D_Viewer");
  settings.setValue("lineWidth", openGLWidget->lineWidth);
  settings.setValue("vertexWidth", openGLWidget->vertexWidth);
  settings.setValue("vertexType", openGLWidget->vertexType);
  settings.setValue("lineType", openGLWidget->lineType);
  settings.setValue("projectionType", openGLWidget->projectionType);
  settings.setValue("colorVertex", openGLWidget->colorVertex);
  settings.setValue("colorBG", openGLWidget->colorBG);
  settings.setValue("colorLine", openGLWidget->colorLine);
}
