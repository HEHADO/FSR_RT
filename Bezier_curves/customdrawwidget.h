#ifndef CUSTOMDRAWWIDGET_H
#define CUSTOMDRAWWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPen>
#include "bezierutils.h"



class CustomDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomDrawWidget(QWidget * parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);

    void changedColor(QColor color);


    virtual void mousePressEvent(QMouseEvent * pe);
    // работа с мышью
    //virtual void mouseMoveEvent(QMouseEvent * pe);
    virtual void mouseReleaseEvent(QMouseEvent * pe);
    //virtual void wheelEvent(QMouseEvent * pe);
    //virtual void mouseDoubleClickEvent(QMouseEvent * pe);

    void clearVector();
    void setDegree(int d);
    QVector<QPointF> vecPoint;
    QVector<QPointF> hullPoints;
    QVector<QPointF> Polygon1;
    QColor m_color;

    int m_id = 0;

    bool layMove = false;


    double scale;
    QVector<QPolygonF> resultPolygons; // Результирующие многоугольники
    bool operationPerformed = false;   // Флаг выполнения операции

signals:
    void dataChanged(QPointF point);
private:
    int degree = 4; // Степень по умолчанию

};

#endif // CUSTOMDRAWWIDGET_H
