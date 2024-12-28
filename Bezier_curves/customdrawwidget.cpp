
#include "bezierutils.h"
#include "customdrawwidget.h"
#include <QPainter>
#include <iostream>

CustomDrawWidget::CustomDrawWidget(QWidget * parent) : QWidget(parent)
{
    m_color = Qt::red;

    scale = 10;

}



void CustomDrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::red, 2);
    painter.setPen(pen);

    // Рисуем изначальную ломаную кривую
    for (int i = 0; i < vecPoint.size() - 1; ++i) {
        painter.drawLine(vecPoint[i], vecPoint[i + 1]);
    }

    // Отображение контрольных точек
    QBrush pointBrush(Qt::green);
    for (const QPointF& point : vecPoint) {
        painter.setBrush(pointBrush);
        painter.drawEllipse(point, 3, 3);
    }

       QVector<QPointF> NumPCurve = generateManualBezier(vecPoint, 100);
        painter.setPen(QPen(Qt::yellow, 2));
        for (int i = 0; i < NumPCurve.size() - 1; ++i) {
            painter.drawLine(NumPCurve[i], NumPCurve[i + 1]);

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

        vecPoint.push_back(newPoint);

        // Эмитируем сигнал для передачи новой точки
        emit dataChanged(newPoint);
    }
    else if (pe->button() == Qt::RightButton)
    {
        layMove = true;


    }

    update();  // Обновление виджета для отображения новых изменений
}



void CustomDrawWidget::mouseReleaseEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        layMove = false;
    }
}

void CustomDrawWidget::clearVector()
{
    vecPoint.clear();
    Polygon1.clear();
    resultPolygons.clear();

    repaint();
}


