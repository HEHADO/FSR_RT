#include "geometryutils.h"
#include "polygonops.h"
#include "triangulation.h"
#include "customdrawwidget.h"
#include <QPainter>
#include <iostream>

CustomDrawWidget::CustomDrawWidget(QWidget * parent) : QWidget(parent)
{
    m_color = Qt::red;

    scale = 10;

}

double CustomDrawWidget::normalizeValue(const LongDouble value, const LongDouble maxValue, double maxRange)
{
    return ((value / maxValue).toDouble() * maxRange);
}

void CustomDrawWidget::paintEvent(QPaintEvent *event)
{
    if (typeTask == task1)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);

        int size_ = vecPoint.size();

        switch (size_)
        {
        case 0:
            break;
        case 1:
            painter.drawEllipse(vecPoint[0].x()-2.5, vecPoint[0].y()-2.5, 5,5);
            break;
        case 2:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            break;
        case 3:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            painter.drawEllipse(vecPoint[2].x()-2.5, vecPoint[2].y()-2.5, 5,5);
            break;
        case 4:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            painter.drawLine(vecPoint[2], vecPoint[3]);
            break;
        }
    }
    else if (typeTask == task2)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);


        if (vecPoint.isEmpty()) {
            return;
        }
        int size_ = vecPoint.size();

        double MaxX = 1, MaxY = 1;
        for(int i = 0; i < size_; ++i){
            QPointF point1 = vecPoint[i];
            if(point1.x() > MaxX) {
                MaxX = point1.x();
            }
            if(point1.y() > MaxY) {
                MaxY = point1.y();
            }
        }
        for (int i = 0; i < size_; ++i) {
            QPointF point = vecPoint[i];

            if (point.x() < 0 || point.x() > 850 || point.y() < 0 || point.y() > 300) {
                // Нормализуем только точки, которые выходят за пределы виджета
                double normalizedX = normalizeValue(LongDouble(point.x()), LongDouble(MaxX), 850);
                double normalizedY = normalizeValue(LongDouble(point.y()), LongDouble(MaxY), 300);
                point = QPointF(normalizedX, normalizedY);
            }

            // Масштабируем точки
            //point *= scale;
            vecPoint[i] = point;
        }
        // Отрисовываем точки и линии с учётом нового масштаба
        switch (size_)
        {
        case 0:
            break;
        case 1:
            painter.drawEllipse(vecPoint[0].x()-2.5 , vecPoint[0].y()-2.5, 5,5);
            break;
        case 2:
            painter.drawLine(vecPoint[0] , vecPoint[1]);
            break;
        case 3:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            painter.drawEllipse(vecPoint[2].x()-2.5, vecPoint[2].y()-2.5, 5, 5);
            break;
        }

    }
    else if (typeTask == task3)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);

        // Рисуем все точки
        for (auto point : vecPoint)
        {
            painter.drawEllipse(point.x()-2.5, point.y()-2.5, 5, 5);
        }

        // Меняем цвет для выпуклой оболочки
        QPen hullPen;
        hullPen.setColor(Qt::blue); // Например, синий цвет для выпуклой оболочки
        hullPen.setWidth(2);
        painter.setPen(hullPen);

        // Рисуем линии выпуклой оболочки
        for (int i = 0; i < hullPoints.size(); ++i)
        {
            painter.drawEllipse(hullPoints[i].x()-2.5, hullPoints[i].y()-2.5, 5, 5);
            if (i < hullPoints.size() - 1) {
                painter.drawLine(hullPoints[i], hullPoints[i + 1]);
            } else {
                painter.drawLine(hullPoints[i], hullPoints[0]); // Соединяем последнюю точку с первой
            }
        }
    }
    else if (typeTask == task4) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);

        // Рисуем все точки
        for (auto point : vecPoint)
        {
            painter.drawEllipse(point.x()-2.5, point.y()-2.5, 5, 5);
        }

        // Отрисовка рёбер триангуляции
        QPen triangulationPen(Qt::blue);
        triangulationPen.setWidth(1);
        painter.setPen(triangulationPen);

        for (const Edge &edge : triangulationEdges)
        {
            painter.drawLine(edge.p1, edge.p2);
        }
    }

    else if (typeTask == task5)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(1.5);
        painter.setPen(pen);

        // Рисуем первый многоугольник
        if (!Polygon1.isEmpty()) {
            painter.drawPolyline(Polygon1);
            for (const QPointF &point : Polygon1) {
                painter.drawEllipse(point, 3, 3);
            }
        }

        // Рисуем второй многоугольник
        QPen pen2;
        pen2.setColor(Qt::blue);
        pen2.setWidth(1.5);
        painter.setPen(pen2);

        if (!Polygon2.isEmpty()) {
            painter.drawPolyline(Polygon2);
            for (const QPointF &point : Polygon2) {
                painter.drawEllipse(point, 3, 3);
            }
        }

        // Если операция выполнена, рисуем результат
        if (operationPerformed) {
            //QBrush brush(Qt::green, Qt::SolidPattern);
            //painter.setBrush(brush);
            QPen resultPen(Qt::green);
            resultPen.setWidth(1.5);
            painter.setPen(resultPen);

            for (const QPolygonF &poly : resultPolygons) {
                painter.drawPolygon(poly);
            }
        }

    }
    else if (typeTask == task6){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(1.5);
        painter.setPen(pen);

        // Рисуем многоугольник
        if (!Polygon1.isEmpty()) {
            painter.drawPolyline(Polygon1);
            for (const QPointF &point : Polygon1) {
                painter.drawEllipse(point, 3, 3);
            }
        }
        QPen pen2;
        pen2.setColor(Qt::green);
        pen2.setWidth(1.5);
        painter.setPen(pen2);
        if (!vecPoint.isEmpty()) {
            painter.drawEllipse(vecPoint[0].x()-2.5 , vecPoint[0].y()-2.5, 5,5);
        }

    }
}

void CustomDrawWidget::changedColor(QColor color)
{
   m_color = color;
   repaint();
}


void CustomDrawWidget::mousePressEvent(QMouseEvent *pe)
{
   if (pe->button() == Qt::LeftButton)
   {

        QPointF newPoint = pe->position();

        switch (typeTask)
        {
        case task1:
            if (vecPoint.size() >= 4) {
                vecPoint.clear();
            }
            vecPoint.push_back(newPoint);
            break;

        case task2:
            if (vecPoint.size() >= 3) {
                vecPoint.clear();
            }
            vecPoint.push_back(newPoint);
            break;
        case task3:
            vecPoint.push_back(newPoint);
            break;
        case task4:
            vecPoint.push_back(newPoint);
            break;
        case task5:
            if (operationPerformed) {
                // Если операция уже выполнена, очищаем данные для новой операции
                clearVector();
                operationPerformed = false;
            }
            if (isFirstPolygon) {
                Polygon1.push_back(newPoint);
            } else {
                Polygon2.push_back(newPoint);
            }
            break;

        case task6:
            if (operationPerformed) {
                // Если операция уже выполнена, очищаем данные для новой операции
                clearVector();
                operationPerformed = false;
            }
            if (isFirstPolygon) {
                Polygon1.push_back(newPoint);
            } else {
                vecPoint.push_back(newPoint);
            }
            break;
        default:
            break;
        }

        // Эмитируем сигнал для передачи новой точки
        emit dataChanged(newPoint);
   }
   else if (pe->button() == Qt::RightButton)
   {
        layMove = true;

        if (typeTask == task1 || typeTask == task2 || typeTask == task3)
        {
            if (!vecPoint.isEmpty())
            {
                double radius = 10;
                double closestDistance = std::numeric_limits<double>::max();
                int closestIndex = -1;

                for (int i = 0; i < vecPoint.size(); ++i)
                {
                    double distanceToCurrent = dist(vecPoint[i], pe->localPos());
                    if (distanceToCurrent < radius && distanceToCurrent < closestDistance)
                    {
                        closestDistance = distanceToCurrent;
                        closestIndex = i;
                    }
                }
                m_id = closestIndex;
            }
        }
   }

   update();  // Обновление виджета для отображения новых изменений
}


void CustomDrawWidget::mouseDoubleClickEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        if (typeTask == task5)
        {
            isFirstPolygon = false;
        }
    }
}


void CustomDrawWidget::mouseMoveEvent(QMouseEvent * pe)
{
    if (layMove == true)
    {
        if (typeTask == task1 or typeTask == task2 or typeTask == task3)
            if (m_id != -1)
            {
                vecPoint[m_id] = pe->position();
                repaint();
            }
        if (typeTask == task5)
        {

        }
    }
}
void CustomDrawWidget::mouseReleaseEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        layMove = false;
    }
}
void CustomDrawWidget::wheelEvent(QMouseEvent * pe)
{

}

void CustomDrawWidget::clearVector()
{
    vecPoint.clear();
    if(typeTask == task3){
        hullPoints.clear();
    }
    if(typeTask == task4){
        triangulationEdges.clear();
    }
    if (typeTask == task5)
    {
        isFirstPolygon = true;
        Polygon1.clear();
        Polygon2.clear();
        resultPolygons.clear();
    }
    if (typeTask == task6)
    {
        isFirstPolygon = true;
        Polygon1.clear();
    }
    repaint();
}

void CustomDrawWidget::getResultOfTasks()
{
    if (typeTask == task1)
    {

    }
    if (typeTask == task2)
    {

    }
    if (typeTask == task3)
    {

    }
    if (typeTask == task4)
    {

    }
    if (typeTask == task5)
    {

    }
    if (typeTask == task6)
    {

    }

}
