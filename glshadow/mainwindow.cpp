#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    glWidget = new OGLWidget(this);
    ui->vLayoutGraphic->addWidget(glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_saveScreenBtn_clicked()
{
    QString filename;
    filename=QFileDialog::getSaveFileName(this, tr("save"),
                                          ".",
                                          tr("Images (*.png *.jpg)"));
    glWidget->saveFbo(filename);
}
