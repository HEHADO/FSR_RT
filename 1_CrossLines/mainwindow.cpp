#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "geometryutils.h"
#include "polygonops.h"
#include "triangulation.h"
#include "longdouble.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget, &CustomDrawWidget::dataChanged,this,&MainWindow::getMouseCoord);

    ui->red_Button->setChecked(true);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getResult_clicked()
{
    ui->widget->getResultOfTasks();
    switch (currentTaskType) {
    case Task1: {
        if (points.size() < 4) {
            ui->textBrowser->setText("Отрезки не заданы полность.");
            return;
        }

        // Объявляем переменную для хранения точки пересечения
        QPointF intersection;
        QString tmp = QString("Size of points Vec(%1).")
                             .arg(points.size());
        ui->textBrowser->append(tmp);
        // Проверяем пересечение и вычисляем точку пересечения
        bool intersect = doIntersect(points[0], points[1], points[2], points[3], intersection);
        if (intersect) {
            QString result = QString("Отрезки пересекаются в точке (%1, %2).")
                                 .arg(intersection.x())
                                 .arg(intersection.y());
            ui->textBrowser->append(result);
        } else {
            ui->textBrowser->append("Не пересекаются.");
        }
        break;
    }
    case Task2: {
        QString tmp = QString("Size of points Vec(%1).")
                          .arg(points.size());
        ui->textBrowser->append(tmp);
        if (points.size() < 3) {
            ui->textBrowser->setText("Точки не заданы полностью для предиката поворота.");
            return;
        }
        int o = orientation(points[0], points[1], points[2]);
        QString result;
        if (o == 0) {
            result = "Straight.";
        } else if (o == 1) {
            result = "Right.";
        } else {
            result = "Left.";
        }
        ui->textBrowser->append(result);
        break;
    }
    case Task3: {
        if (points.size() < 3) {
            ui->textBrowser->append("Недостаточно точек для построения выпуклой оболочки.");
            return;
        }

        // Вызываем алгоритм Джарвиса для построения выпуклой оболочки
        QVector<QPointF> hull = jarvisConvexHull(points);

        // Сохраняем точки выпуклой оболочки в отдельный вектор
        ui->widget->hullPoints = hull;

        // Обновляем виджет для отображения новой оболочки
        ui->widget->update();

        // Отображаем информацию о построенной оболочке в textBrowser
        ui->textBrowser->append("Выпуклая оболочка построена. Количество точек: " + QString::number(hull.size()));
        break;
    }
    case Task4:
    {
        if (points.size() < 3) {
            ui->textBrowser->append("Недостаточно точек для выполнения триангуляции.");
            return;
        }
        QStringList logMessages;  // Логи рёбер

        QVector<Edge> triangulationEdges = getTriangulation(points, logMessages);

        // Выводим логированные сообщения
        ui->textBrowser->append("Логи рёбер:");
        for (const QString &msg : logMessages) {
            ui->textBrowser->append(msg);
        }
        // Устанавливаем рёбра триангуляции для отрисовки
        ui->widget->triangulationEdges = triangulationEdges;
        ui->widget->update();

        // Отображаем информацию о триангуляции
        ui->textBrowser->append("Триангуляция выполнена. Количество рёбер: " + QString::number(triangulationEdges.size()));
        break;
    }
    case Task5:
        ui->textBrowser->setText("Задача для работы с произвольными многоугольниками.");
        break;
    case Task6:
        ui->textBrowser->setText("Задача локализации точки.");
        if (ui->widget->Polygon1.size() < 3) {
            ui->textBrowser->append("Недостаточно точек для многоугольника.");
            return;
        }

        ui->widget->Polygon1.append(ui->widget->Polygon1.first());
        ui->widget->update();
        ui->textBrowser->append("Многоугольник завершён. Ставим точку.");
        if (ui->widget->vecPoint.isEmpty()) {
            ui->textBrowser->append("Точка не задана.");
            return;
        }
        QVector<QPointF> polyPoints = ui->widget->Polygon1;
        QPointF A = ui->widget->vecPoint[0];
        QVector<Edge> seg;
        for (int i = 0; i < polyPoints.size(); i++) {
            seg.append(Edge(polyPoints[i], polyPoints[(i+1)%polyPoints.size()]));
        }
        if(!point_not_in_vec(polyPoints, A)){
            ui->textBrowser->append("Точка на одной из граней многоугольника");
            ui->widget->operationPerformed = true;
        }else if(in_figure(seg, A)){
            ui->textBrowser->append("Точка внутри многоугольника");
            ui->widget->operationPerformed = true;

        }else{
            ui->textBrowser->append("Точка снаружи многоугольника");
            ui->widget->operationPerformed = true;
        }
        break;
    }
}

void MainWindow::getMouseCoord(QPointF point)
{
    points.append(point);
    double x_ = point.x();
    double y_ = point.y();
    QString textOutPut = "x_: " + QString::number(x_) + " y_: " +
            QString::number(y_);
    ui->textBrowser->append(textOutPut);
}

void MainWindow::on_Add_point_manual_clicked()
{
    // Читаем значения x и y из текстовых полей
    QString xStr = ui->Xcoord->toPlainText();  // Используем toPlainText(), так как это QTextEdit
    QString yStr = ui->Ycoord->toPlainText();

    // Преобразуем строковые значения в LongDouble
    LongDouble x, y;

    try {
        x = LongDouble(xStr.toStdString());
        y = LongDouble(yStr.toStdString());
        ui->textBrowser->append("Добаление успешно, преобразование сделано");
    } catch (const std::exception& e) {
        ui->textBrowser->append("Ошибка: Неверный формат координат");
        return;
    }

    // Создаём точку и добавляем её в вектор точек
    QPointF newPoint(x.toDouble(), y.toDouble());  // Используем toDouble() для создания точки, которую можно отобразить на виджете
    points.append(newPoint);
    ui->widget->vecPoint.push_back(newPoint);

    // Обновляем виджет для отображения новой точки
    ui->widget->update();

    // Выводим сообщение об успешном добавлении точки
    ui->textBrowser->append(QString("Добавлена точка: (%1, %2)").arg(xStr).arg(yStr));
}

void MainWindow::on_red_Button_clicked()
{
    ui->green_Button->setChecked(false);
    ui->widget->changedColor(Qt::red);
}

void MainWindow::on_green_Button_clicked()
{
    ui->red_Button->setChecked(false);
    ui->widget->changedColor(Qt::green);
}

void MainWindow::on_pushButton_deletePoints_clicked()
{
    // Очищаем вектор с точками
    points.clear();

    // Обновляем интерфейс для отображения изменений
    ui->textBrowser->append("Все точки удалены.");
    ui->widget->clearVector();
    ui->textBrowser->clear();
}

void MainWindow::on_task1_clicked()
{
    ui->widget->setType(task1);
    currentTaskType = Task1;
    points.clear();
    ui->textBrowser->setText("Выбрано: Пересечение двух отрезков");

}

void MainWindow::on_task3_clicked()
{
    ui->widget->setType(task3);
    currentTaskType = Task3;
    points.clear();
    ui->textBrowser->setText("Выбрано: Построение выпуклой оболочки");
}



void MainWindow::on_task2_clicked()
{
    ui->widget->setType(task2);
    currentTaskType = Task2;
    points.clear();
    ui->textBrowser->setText("Выбрано: Предикат поворота");
}

void MainWindow::on_task4_clicked()
{
    ui->widget->setType(task4);
    currentTaskType = Task4;
    points.clear();
    ui->textBrowser->setText("Выбрано: Триангуляция Делоне");
}

void MainWindow::on_task5_clicked()
{
   ui->widget->setType(task5);
    currentTaskType = Task5;
    points.clear();
    ui->textBrowser->setText("Выбрано: Пересечение произвольных многоугольников");
}

void MainWindow::on_task6_clicked()
{
    ui->widget->setType(task6);
   currentTaskType = Task6;
   points.clear();
   ui->textBrowser->setText("Выбрано: Локализация точки относительно многоугольника");
}


void MainWindow::on_New_Poly_clicked()
{
   if (currentTaskType == Task5)
   {
        ui->widget->isFirstPolygon = false;
        // Замыкаем первый многоугольник
        int N = ui->widget->Polygon1.size();
        if (N > 1 && ui->widget->Polygon1.first() != ui->widget->Polygon1.last()) {
            ui->widget->Polygon1.append(ui->widget->Polygon1.first());
        }
        ui->widget->update();
        ui->textBrowser->append("Первый многоугольник завершён. Рисуем второй многоугольник.");
   }
}

void MainWindow::on_Add_point_clicked()
{
   if(currentTaskType == Task6)
   {
        ui->widget->isFirstPolygon = false;
        // Замыкаем первый многоугольник
        if (!ui->widget->Polygon1.isEmpty()) {
            ui->widget->Polygon1.append(ui->widget->Polygon1.first());
        }
        ui->widget->update();
        ui->textBrowser->append("Многоугольник завершён. Ставим точку.");

   }
}

void MainWindow::on_intersection_calc_clicked()
{
   if (currentTaskType == Task5)
   {
        // Замыкаем второй многоугольник
        if (!ui->widget->Polygon2.isEmpty()) {
            int M = ui->widget->Polygon2.size();
            if (M > 1 && ui->widget->Polygon2.first() != ui->widget->Polygon2.last()) {
                ui->widget->Polygon2.append(ui->widget->Polygon2.first());
            }
        }
//        if (!isConvexPolygon(ui->widget->Polygon1) || !isConvexPolygon(ui->widget->Polygon2)) {
//            ui->textBrowser->append("Ошибка: Один из многоугольников не является выпуклым!");
//            return;
//        }
        QVector<Edge> segA;
        QVector<Edge> segB;

        QVector<QPointF> &poly1 = ui->widget->Polygon1;
        QVector<QPointF> &poly2 = ui->widget->Polygon2;

        int NA = poly1.size();
        if (NA > 1 && ui->widget->triangulationEdges.isEmpty()) {
            // Комментарий: не проверяем triangulationEdges без надобности, просто формируем отрезки
            segA.clear();
            for (int i = 0; i < NA - 1; ++i) {
                segA.append(Edge(poly1[i], poly1[i+1]));
            }
        }

        int NB = poly2.size();
        if (NB > 1) {
            segB.clear();
            for (int i = 0; i < NB - 1; ++i) {
                segB.append(Edge(poly2[i], poly2[i+1]));
            }
        }

        QVector<Edge> resultEdges = do_intersection(segA, segB);

        // Если resultEdges не пуст, формируем результат:
        QVector<QPointF> resultPoly;
        if (!resultEdges.isEmpty()) {
            // Начинаем с resultEdges[0].p1
            resultPoly.append(resultEdges[0].p1);
            for (int i = 0; i < resultEdges.size(); i++) {
                resultPoly.append(resultEdges[i].p2);
            }
        }
        ui->widget->resultPolygons.clear();
        ui->widget->resultPolygons.append(QPolygonF(resultPoly));
        ui->widget->operationPerformed = true;
        ui->widget->update();
        ui->textBrowser->append("Пересечение многоугольников вычислено и отображено.");
   }
}


void MainWindow::on_combining_calc_clicked()
{
   if (currentTaskType == Task5)
   {
        // Замыкаем второй многоугольник
        if (!ui->widget->Polygon2.isEmpty()) {
            int M = ui->widget->Polygon2.size();
            if (M > 1 && ui->widget->Polygon2.first() != ui->widget->Polygon2.last()) {
                ui->widget->Polygon2.append(ui->widget->Polygon2.first());
            }
        }

        QVector<Edge> segA;
        QVector<Edge> segB;

        QVector<QPointF> &poly1 = ui->widget->Polygon1;
        QVector<QPointF> &poly2 = ui->widget->Polygon2;

        int NA = poly1.size();
        if (NA > 1 && ui->widget->triangulationEdges.isEmpty()) {
            // Комментарий: не проверяем triangulationEdges без надобности, просто формируем отрезки
            segA.clear();
            for (int i = 0; i < NA - 1; ++i) {
                segA.append(Edge(poly1[i], poly1[i+1]));
            }
        }

        int NB = poly2.size();
        if (NB > 1) {
            segB.clear();
            for (int i = 0; i < NB - 1; ++i) {
                segB.append(Edge(poly2[i], poly2[i+1]));
            }
        }

        QVector<Edge> resultEdges = do_union(segA, segB, poly1[0], poly2[0]);

        // Если resultEdges не пуст, формируем результат:
        QVector<QPointF> resultPoly;
        if (!resultEdges.isEmpty()) {
            // Чужой код: предположение, что resultEdges образуют замкнутый многоугольник.
            // Начинаем с resultEdges[0].p1
            resultPoly.append(resultEdges[0].p1);
            for (int i = 0; i < resultEdges.size(); i++) {
                resultPoly.append(resultEdges[i].p2);
            }
        }
        ui->widget->resultPolygons.clear();
        ui->widget->resultPolygons.append(QPolygonF(resultPoly));
        ui->widget->operationPerformed = true;
        ui->widget->update();
        ui->textBrowser->append("Пересечение многоугольников вычислено и отображено.");

   }
}

void MainWindow::on_difference_calc_clicked()
{
   if (currentTaskType == Task5)
   {
        // Замыкаем второй многоугольник
        if (!ui->widget->Polygon2.isEmpty()) {
            int M = ui->widget->Polygon2.size();
            if (M > 1 && ui->widget->Polygon2.first() != ui->widget->Polygon2.last()) {
                ui->widget->Polygon2.append(ui->widget->Polygon2.first());
            }
        }

        QVector<Edge> segA;
        QVector<Edge> segB;

        QVector<QPointF> &poly1 = ui->widget->Polygon1;
        QVector<QPointF> &poly2 = ui->widget->Polygon2;

        int NA = poly1.size();
        if (NA > 1 && ui->widget->triangulationEdges.isEmpty()) {
            // Комментарий: не проверяем triangulationEdges без надобности, просто формируем отрезки
            segA.clear();
            for (int i = 0; i < NA - 1; ++i) {
                segA.append(Edge(poly1[i], poly1[i+1]));
            }
        }

        int NB = poly2.size();
        if (NB > 1) {
            segB.clear();
            for (int i = 0; i < NB - 1; ++i) {
                segB.append(Edge(poly2[i], poly2[i+1]));
            }
        }

        QVector<Edge> resultEdges = do_difference(segA, segB);

        // Если resultEdges не пуст, формируем результат:
        QVector<QPointF> resultPoly;
        if (!resultEdges.isEmpty()) {
            // Чужой код: предположение, что resultEdges образуют замкнутый многоугольник.
            // Начинаем с resultEdges[0].p1
            resultPoly.append(resultEdges[0].p1);
            for (int i = 0; i < resultEdges.size(); i++) {
                resultPoly.append(resultEdges[i].p2);
            }
        }
        ui->widget->resultPolygons.clear();
        ui->widget->resultPolygons.append(QPolygonF(resultPoly));
        ui->widget->operationPerformed = true;
        ui->widget->update();
        ui->textBrowser->append("Пересечение многоугольников вычислено и отображено.");


   }
}

