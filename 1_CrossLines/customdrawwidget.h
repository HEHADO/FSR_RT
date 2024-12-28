#ifndef CUSTOMDRAWWIDGET_H
#define CUSTOMDRAWWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPen>
#include "geometryutils.h"
#include "longdouble.h"


enum
{
    notask = 0,
    task1 = 1,
    task2 = 2,
    task3 = 3,
    task4 = 4,
    task5 = 5,
    task6 = 6,
};

class CustomDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomDrawWidget(QWidget * parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);

    void changedColor(QColor color);


    virtual void mousePressEvent(QMouseEvent * pe);
    // работа с мышью
    virtual void mouseMoveEvent(QMouseEvent * pe);
    virtual void mouseReleaseEvent(QMouseEvent * pe);
    virtual void wheelEvent(QMouseEvent * pe);
    virtual void mouseDoubleClickEvent(QMouseEvent * pe);

    void clearVector();

    void getResultOfTasks();

    void setType(int type) { typeTask = type; clearVector();}

    QVector<QPointF> vecPoint;
    QVector<QPointF> hullPoints;
    QVector<QPointF> Polygon1;
    QVector<QPointF> Polygon2;
    bool isFirstPolygon = true;
    QVector<Edge> triangulationEdges;
    QColor m_color;

    int typeTask = notask;
    int m_id = 0;

    bool layMove = false;

    double scale;
    QVector<QPolygonF> resultPolygons; // Результирующие многоугольники
    bool operationPerformed = false;   // Флаг выполнения операции

signals:
    void dataChanged(QPointF point);
private:
    double normalizeValue(const LongDouble value, const LongDouble maxValue, double maxRange);
};

#endif // CUSTOMDRAWWIDGET_H
