#include "triangulation.h"

bool check(Edge s0, QVector <Edge> s){
    bool flag = true; // Assume the segment is not in the list
    Edge s1; // Create reverse segment for comparison
    s1.p1 = s0.p2;
    s1.p2 = s0.p1;
    for(int i = 0; i < s.size(); i++)
        if (s0 == s[i] || s1 == s[i]) // If either segment or its reverse is found
            flag = false; // Segment is in the list
    return flag; // Return true if segment is not in the list, false otherwise
}


QVector<Triangle> delaunayTriangulation(const QVector<QPointF>& points) { //триангуляция
    // Создаем супер-треугольник
    double minX = points[0].x();
    double minY = points[0].y();
    double maxX = minX;
    double maxY = minY;

    for (const QPointF& p : points) {
        if (p.x() < minX) minX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() > maxY) maxY = p.y();
    }

    double dx = maxX - minX;
    double dy = maxY - minY;
    double deltaMax = std::max(dx, dy);
    double midx = (minX + maxX) / 2.0;
    double midy = (minY + maxY) / 2.0;

    QPointF p1(midx - 20 * deltaMax, midy - deltaMax);
    QPointF p2(midx, midy + 20 * deltaMax);
    QPointF p3(midx + 20 * deltaMax, midy - deltaMax);

    QVector<Triangle> triangulation;
    triangulation.append(Triangle(p1, p2, p3));

    // Добавим точек в триангуляцию
    for (const QPointF& p : points) {
        QVector<Triangle> badTriangles;
        QVector<Edge> polygon;

        // Ищем лишние треугольники
        for (const Triangle& t : triangulation) {
            if (t.containsPoint(p)) {
                badTriangles.append(t);
            }
        }

        // Ищем границу многоугольника
        QSet<Edge> edgeCount;
        for (const Triangle& t : badTriangles) {
            Edge e1(t.p1, t.p2);
            Edge e2(t.p2, t.p3);
            Edge e3(t.p3, t.p1);

            if (edgeCount.contains(e1)) {
                edgeCount.remove(e1);
            } else {
                edgeCount.insert(e1);
            }

            if (edgeCount.contains(e2)) {
                edgeCount.remove(e2);
            } else {
                edgeCount.insert(e2);
            }

            if (edgeCount.contains(e3)) {
                edgeCount.remove(e3);
            } else {
                edgeCount.insert(e3);
            }
        }

        // Удаляем лишние треугольники
        for (const Triangle& t : badTriangles) {
            triangulation.removeAll(t);
        }

        // Хоба, новые треугольники
        for (const Edge& e : edgeCount) {
            triangulation.append(Triangle(e.p1, e.p2, p));
        }
    }

    // Удаляем треугольники, связанные с супер-треугольником
    QVector<Triangle> finalTriangulation;
    for (const Triangle& t : triangulation) {
        if (t.p1 == p1 || t.p1 == p2 || t.p1 == p3 ||
            t.p2 == p1 || t.p2 == p2 || t.p2 == p3 ||
            t.p3 == p1 || t.p3 == p2 || t.p3 == p3) {
            continue;
        }
        finalTriangulation.append(t);
    }

    return finalTriangulation;
}


QVector<Edge> getTriangulation(const QVector<QPointF> &points, QStringList &logMessages) { //Подготовка и вызов
    QVector<Edge> edges;

    QVector<Triangle> triangles = delaunayTriangulation(points);

    QSet<Edge> edgeSet;
    for (const Triangle &t : triangles) {
        Edge e1(t.p1, t.p2);
        Edge e2(t.p2, t.p3);
        Edge e3(t.p3, t.p1);

        // Добавляем рёбра в набор, чтобы избежать дубликатов
        edgeSet.insert(e1);
        edgeSet.insert(e2);
        edgeSet.insert(e3);
    }

    edges = edgeSet.values().toVector();

    // Логируем рёбра
    for (const Edge &e : edges) {
        QString info = QString("Ребро: (%1, %2) -> (%3, %4)")
                           .arg(e.p1.x())
                           .arg(e.p1.y())
                           .arg(e.p2.x())
                           .arg(e.p2.y());
        logMessages.append(info);
    }

    return edges;
}

