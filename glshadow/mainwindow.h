#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oglwidget.h"
#include "stlloader.h"
#include <QProgressDialog>
#include <QTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_saveScreenBtn_clicked();

private:
    OGLWidget *glWidget;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
