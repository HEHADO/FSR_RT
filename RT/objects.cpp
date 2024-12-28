#include "objects.h"


//
// Конструкторы Vec3f
//

Vec3f:: Vec3f(){}

Vec3f:: Vec3f(double inX, double inY, double inZ): x(inX), y(inY), z(inZ){

}

//
// Функции Vec3f
//


//
// Конструкторы Color
//

Color:: Color() : R(0), G(0), B(0) { }
Color:: Color(unsigned short iR,
      unsigned short iG,
      unsigned short iB) : R(iR), G(iG), B(iB) { }


//
// Конструкторы Ray
//

Ray::Ray() { }
Ray:: Ray(const Vec3f& b, const Vec3f& d) : beg(b), dir(d) { }

//
// Конструкторы LightSourse
//

LightSource:: LightSource() { }
LightSource:: LightSource(int t, const Vec3f& v, const double& intens)
    : type(t), position(v), intensity(intens) { }


//
// Конструкторы Object
//

Object:: Object() { }
Object:: Object(Color c) : color(c) { }


//
// Конструкторы Sphere
//


Sphere:: Sphere() : orbitRadius(0), angle(0), angleStep(0) { }
Sphere:: Sphere(const double& rad, const Vec3f& v): radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300), orbitRadius(0), angle(0), angleStep(0) { }
Sphere:: Sphere(const double& rad,
       const Vec3f& v,
       Color c)  : Object(c), radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300), orbitRadius(0), angle(0), angleStep(0) {r=-1; s=-1;}


Sphere:: Sphere(const double& rad,
       const Vec3f& v,
       Color c,  double spect, double refl) : Object(c), radius(rad), center(v), dir(0,1,0), step(5), phase(0), amp(300), orbitRadius(0), angle(0), angleStep(0) {r = refl; s = spect;}


Sphere:: Sphere(const double& rad,
       const Vec3f& v,
       Color c,
       double in_step,
       Vec3f in_dir,
       double in_amp) :
    Object(c), radius(rad), center(v),
    dir(in_dir), step(in_step), phase(0), amp(in_amp), orbitRadius(0), angle(0), angleStep(0) { }
//
// Функции Sphere
//


Vec3f Sphere:: get_normal(const Vec3f& v) const
{
    return (v - center).Normilize();
}

Vec3f Sphere:: gep_pos() const
{
    return center + phase * dir;
}

double Sphere:: is_intersect(const Ray& r) const
{
    double A = r.dir.sq();
    Vec3f v = r.beg - gep_pos();
    double B = r.dir * v;
    double C = v.sq() - radius * radius;

    double D = B * B - A * C;

    if(D < 0)
    {
        return -1.0;
    }
    double sD = sqrt(D);
    double t1 = (-B - sD) / A;
    double t2 = (-B + sD) / A;

    return (t1 > 0) ? t1 : t2;
}

void Sphere:: tick()
{
    // Если orbitRadius > 0, значит сфера движется по орбите
    if(orbitRadius > 0)
    {
        // Фиксируем высоту
        double fixedY = center.y;

        angle += angleStep;
        // Движение по кругу в плоскости XZ:
        double newX = orbitCenter.x + orbitRadius * cos(angle);
        double newZ = orbitCenter.z + orbitRadius * sin(angle);

        center = Vec3f(newX, fixedY, newZ);
    }


}

void Sphere:: setOrbit(const Vec3f& orbitC, double radius, double initialAngle, double stepAngle)
{
    orbitCenter = orbitC;
    orbitRadius = radius;
    angle = initialAngle;
    angleStep = stepAngle;
}

//
// Конструкторы Plane3v
//

Plane3v:: Plane3v() : Object(Color()) { } // dummy object
Plane3v:: Plane3v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        Color col) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    s = -1;
    r = -1;
}
Plane3v:: Plane3v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        Color col, double spect, double refl) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    s = spect;
    r = refl;
}


//
// Функции Plane3v
//

double Plane3v:: is_intersect(const Ray& r) const
{
    return is_intersect_tr(r, vert[0], vert[1], vert[2]);

}

Vec3f Plane3v:: get_normal(const Vec3f& v) const
{
    return ((vert[2] - vert[0]) % (vert[1] - vert[0])).Normilize();
}

//
// Конструкторы Plane4v
//

Plane4v:: Plane4v() : Object(Color()) { }
Plane4v:: Plane4v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        const Vec3f& D,
        Color col) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    vert[3] = D;
    s = -1;
    r = -1;
}
Plane4v:: Plane4v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        const Vec3f& D,
        Color col, double spect, double refl) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    vert[3] = D;
    s = spect;
    r = refl;
}

//
// Функции Plane4v
//

double Plane4v:: is_intersect(const Ray& r) const
{
    double t1 = is_intersect_tr(r, vert[0], vert[1], vert[2]);
    if(t1 > 0)
        return t1;

    return is_intersect_tr(r, vert[2], vert[3], vert[0]);
}

Vec3f Plane4v:: get_normal(const Vec3f& v) const
{
    return ((vert[2] - vert[0]) % (vert[1] - vert[0])).Normilize();
}

