#include "bezierutils.h"
#include <cmath>




double binomial_coefficient(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;

    double result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

QVector<QPointF> generateManualBezier(const QVector<QPointF>& points, int segments) {
    QVector<QPointF> result;
    int degree = points.size()-1;
//    if (points.size() < degree + 1) return result; // Проверяем, достаточно ли точек

    for (int i = 0; i <= segments; ++i) {
        double t = static_cast<double>(i) / segments;
        QPointF point(0, 0);

        for (int j = 0; j <= degree; ++j) {
            double coefficient = binomial_coefficient(degree, j) *
                                 std::pow(1 - t, degree - j) *
                                 std::pow(t, j);
            point += coefficient * points[j];
        }

        result.push_back(point);
    }
    return result;
}


