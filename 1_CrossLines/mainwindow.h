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
    enum TaskType {
        Task1,  // Пересечение двух отрезков
        Task2,  // Предикат поворота
        Task3,  // Построение выпуклой оболочки
        Task4,  // Триангуляция Делоне
        Task5,  // Пересечениеn произвольных многоугольников
        Task6   // Локализация точки относительно многоугольника
    };
    TaskType currentTaskType; // Переменная для хранения текущей задачи
    QVector<QPointF> points; // Контейнер для хранения точек
    Ui::MainWindow *ui;

private slots:
    void getMouseCoord(QPointF point);
    void on_Add_point_manual_clicked();
    void on_red_Button_clicked();
    void on_green_Button_clicked();
    void on_pushButton_deletePoints_clicked();
    void on_task1_clicked();
    void on_task3_clicked();
    void on_getResult_clicked();
    void on_task2_clicked();
    void on_task4_clicked();
    void on_task5_clicked();
    void on_task6_clicked();
    void on_New_Poly_clicked();
    void on_Add_point_clicked();
    void on_intersection_calc_clicked();
    void on_difference_calc_clicked();
    void on_combining_calc_clicked();
};

#endif // MAINWINDOW_H
