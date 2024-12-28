#ifndef BEZIERUTILS_H
#define BEZIERUTILS_H
#include <QPointF>
#include <math.h>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <cassert>

#include <QVector>
#include <QSet>
#include <algorithm>



std::vector<double> bezierCoefficients(int d, double t);


QVector<QPointF> generateManualBezier(const QVector<QPointF>& points, int segments);


#endif // BEZIERUTILS_H
