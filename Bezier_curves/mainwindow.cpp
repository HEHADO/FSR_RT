#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "customdrawwidget.h"

#include "bezierutils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget, &CustomDrawWidget::dataChanged,this,&MainWindow::getMouseCoord);

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getMouseCoord(QPointF point)
{
    points.append(point);
    double x_ = point.x();
    double y_ = point.y();
    QString textOutPut = "x_: " + QString::number(x_) + " y_: " +
                         QString::number(y_);

}

