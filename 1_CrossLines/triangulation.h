#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "geometryutils.h" // для базовых функций
#include "polygonops.h"


 bool check(Edge s0, QVector <Edge> s); //проверяем, записано ли уже ребро

 QVector<Triangle> delaunayTriangulation(const QVector<QPointF>& points);

 QVector<Edge> getTriangulation(const QVector<QPointF> &points, QStringList &logMessages);


#endif // TRIANGULATION_H
