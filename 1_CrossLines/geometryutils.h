#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H
#include <QPointF>
#include <math.h>
#include <cmath>
#include <QVector>
#include <QSet>
#include <algorithm>

struct Edge {
    QPointF p1, p2;
    int pred; // для пересечения

    Edge() : pred(0) {}
    Edge(const QPointF& a, const QPointF& b) : p1(a), p2(b), pred(0) {}

    bool operator==(const Edge& other) const {
        return ((p1 == other.p1 && p2 == other.p2) ||
                (p1 == other.p2 && p2 == other.p1));
    }
};

struct Triangle {
    QPointF p1, p2, p3;
    QPointF circumcenter;
    double radiusSquared;
    Triangle() : p1(), p2(), p3(), circumcenter(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()),
        radiusSquared(std::numeric_limits<double>::infinity()) {}
    Triangle(const QPointF& a, const QPointF& b, const QPointF& c) : p1(a), p2(b), p3(c) {
        calculateCircumcircle();
    }
    // Добавляем оператор ==
    bool operator==(const Triangle& other) const {
        // Сравниваем треугольники по их вершинам
        // Поскольку порядок вершин может быть разным, нужно проверить все возможные перестановки
        QVector<QPointF> thisPoints = {p1, p2, p3};
        QVector<QPointF> otherPoints = {other.p1, other.p2, other.p3};

        // Сортируем точки для корректного сравнения
        std::sort(thisPoints.begin(), thisPoints.end(), [](const QPointF& a, const QPointF& b) {
            return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
        });
        std::sort(otherPoints.begin(), otherPoints.end(), [](const QPointF& a, const QPointF& b) {
            return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
        });

        return thisPoints[0] == otherPoints[0] &&
               thisPoints[1] == otherPoints[1] &&
               thisPoints[2] == otherPoints[2];
    }
    void calculateCircumcircle();
    bool containsPoint(const QPointF& p) const;
};


// Статическая функция для вычисления расстояния между двумя точками
inline double dist(const QPointF &A, const QPointF &B) {
    double dx = A.x() - B.x();
    double dy = A.y() - B.y();
    return std::sqrt(dx*dx + dy*dy);
}



inline uint qHash(const QPointF &key, uint seed = 0) {
    // Используем масштабирование для сохранения точности
    qint64 x = qRound64(key.x() * 1000000);
    qint64 y = qRound64(key.y() * 1000000);
    return qHash(x, seed) ^ qHash(y, seed);
}

inline uint qHash(const Edge &key, uint seed = 0) {
    return qHash(key.p1, seed) ^ qHash(key.p2, seed);
}



 bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF &intersection);
 bool onSegment(QPointF p, QPointF q, QPointF r);

 double vectorProduct(QPointF p1, QPointF p2);
 double norma(QPointF vec);
 double angle(QPointF A, QPointF B, QPointF C);

 int orientation(QPointF p, QPointF q, QPointF r);

 QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points);


#endif // GEOMETRYUTILS_H
