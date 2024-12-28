#include "geometryutils.h"
#include "longdouble.h"



void Triangle::calculateCircumcircle() {
    double ax = p1.x();
    double ay = p1.y();
    double bx = p2.x();
    double by = p2.y();
    double cx = p3.x();
    double cy = p3.y();

    double D = 2 * (ax*(by - cy) + bx*(cy - ay) + cx*(ay - by));
    if (D == 0) {
        // Точки коллинеарны
        circumcenter = QPointF(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
        radiusSquared = std::numeric_limits<double>::infinity();
        return;
    }

    double Ux = ((ax*ax + ay*ay)*(by - cy) + (bx*bx + by*by)*(cy - ay) + (cx*cx + cy*cy)*(ay - by)) / D;
    double Uy = ((ax*ax + ay*ay)*(cx - bx) + (bx*bx + by*by)*(ax - cx) + (cx*cx + cy*cy)*(bx - ax)) / D;

    circumcenter = QPointF(Ux, Uy);
    radiusSquared = (Ux - ax)*(Ux - ax) + (Uy - ay)*(Uy - ay);
}

bool Triangle::containsPoint(const QPointF& p) const {
    double dx = p.x() - circumcenter.x();
    double dy = p.y() - circumcenter.y();
    double distSquared = dx*dx + dy*dy;
    return distSquared <= radiusSquared;
}

bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection)
{    // Вычисляем ориентации для четырёх комбинаций точек
    int o1 = orientation(A, B, C);
    int o2 = orientation(A, B, D);
    int o3 = orientation(C, D, A);
    int o4 = orientation(C, D, B);

    // Основное условие: отрезки пересекаются, если ориентации различны
    if (o1 != o2 && o3 != o4) {
        // Вычисляем точку пересечения
        double a1 = B.y() - A.y();
        double b1 = A.x() - B.x();
        double c1 = a1 * A.x() + b1 * A.y();

        double a2 = D.y() - C.y();
        double b2 = C.x() - D.x();
        double c2 = a2 * C.x() + b2 * C.y();

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            double x = (b2 * c1 - b1 * c2) / determinant;
            double y = (a1 * c2 - a2 * c1) / determinant;
            intersection = QPointF(x, y);

            // Дополнительно проверяем, лежит ли точка пересечения на обоих отрезках
            if (onSegment(A, intersection.toPoint(), B) && onSegment(C, intersection.toPoint(), D)) {
                return true;
            }
        }
        return false;
    }

    // Граничные случаи: когда точки лежат на одном отрезке
    if (o1 == 0 && onSegment(A, C, B)) {
        intersection = QPointF(C);
        return true;
    }
    if (o2 == 0 && onSegment(A, D, B)) {
        intersection = QPointF(D);
        return true;
    }
    if (o3 == 0 && onSegment(C, A, D)) {
        intersection = QPointF(A);
        return true;
    }
    if (o4 == 0 && onSegment(C, B, D)) {
        intersection = QPointF(B);
        return true;
    }

    return false;  // Отрезки не пересекаются
}

bool onSegment(QPointF p, QPointF q, QPointF r) {
    return ((q.x() <= std::max(p.x(), r.x())) && (q.x() >= std::min(p.x(), r.x()))&&
            (q.y() <= std::max(p.y(), r.y())) && (q.y() >= std::min(p.y(), r.y())));
}


double vectorProduct(QPointF p1, QPointF p2)
{
    double res = p1.x() * p2.x() + p1.y() * p2.y();
    return res;
}

double norma(QPointF vec)
{
    return sqrt(vec.x() * vec.x() + vec.y() * vec.y());
}

double angle(QPointF A, QPointF B, QPointF C){
    if ((C == A) || (C == B))
        return 0; // No angle if points are overlapping
    A = A - B; // Vector from B to A
    C = C - B; // Vector from B to C
    // Dot product divided by product of magnitudes gives cosine of the angle
    double cosa = (A.x() * C.x() + A.y() * C.y())/(sqrt(pow(A.x(), 2) + pow(A.y(), 2))*sqrt(pow(C.x(), 2) + pow(C.y(), 2)));
    return abs(acos(cosa)); // Return absolute value to ensure angle is positive
}


int orientation(QPointF p, QPointF q, QPointF r) {

    LongDouble pqx = LongDouble(q.x()) - LongDouble(p.x());
    LongDouble pry = LongDouble(r.y()) - LongDouble(p.y());
    LongDouble pqy = LongDouble(q.y()) - LongDouble(p.y());
    LongDouble prx = LongDouble(r.x()) - LongDouble(p.x());

    LongDouble val = (pqx * pry) - (pqy * prx);
    if (val == LongDouble(0)) {
        return 0;  // На прямой
    } else if (val > LongDouble(0)) {
        return 1;  // Правый поворот
    } else {
        return 2;  // Левый поворот
    }
}


QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points) {
    QVector<QPointF> hull;
    if (points.size() < 3) {
        return hull; // Выпуклая оболочка невозможна, если меньше 3 точек
    }

    // Находим самую левую точку
    int l = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i].x() < points[l].x()) {
            l = i;
        }
    }

    int p = l, q;
    do {
        // Добавляем текущую точку в оболочку
        hull.push_back(points[p]);

        // Находим следующую точку, которая образует левый поворот
        q = (p + 1) % points.size();
        for (int i = 0; i < points.size(); i++) {
            if (orientation(points[p], points[i], points[q]) == 2) {
                q = i;
            }
        }

        // Переходим к следующей точке
        p = q;

    } while (p != l); // Пока не вернулись к начальной точке

    return hull;
}
