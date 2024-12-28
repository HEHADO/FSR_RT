#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "customdrawwidget.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:


    QVector<QPointF> points; // Контейнер для хранения точек
    Ui::MainWindow *ui;

private slots:
    void getMouseCoord(QPointF point);



};

#endif // MAINWINDOW_H
