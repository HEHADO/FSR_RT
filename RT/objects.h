#ifndef OBJECTS_H
#define OBJECTS_H

#include <math.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

struct Vec3f
{
    double x;
    double y;
    double z;

    Vec3f();
    Vec3f(double inX, double inY, double inZ);
    Vec3f operator+(const Vec3f& o) const
    {
        return Vec3f(x + o.x, y + o.y, z + o.z);
    }

    Vec3f operator-(const Vec3f& o) const
    {
        return Vec3f(x - o.x, y - o.y, z - o.z);
    }

    Vec3f operator*(const double& d) const
    {
        return Vec3f(x * d, y * d, z * d);
    }

    double operator*(const Vec3f& o) const
    {
        return x * o.x + y * o.y + z * o.z;
    }

    double sq() const
    {
        return x * x + y * y + z * z;
    }

    double length() const
    {
        return sqrt(sq());
    }

    Vec3f operator%(const Vec3f& o) const
    {
        return Vec3f(y * o.z - z * o.y, -x * o.z + z * o.x, x * o.y - y * o.x);
    }
    Vec3f norm(){
        return (*this)*(1/length());
    }

    Vec3f Normilize() const { return (*this) * (1.0 / length()); }
};


struct IntersectionResult {
    double t;
    int planeIndex;
};

inline Vec3f operator*(const double& a, const Vec3f& v)
{
    return v * a;
}

inline double mixed(const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
    return (a % b) * c;
}

class Projector
{
protected:
    double l0; // расстояние до картинной плоскости

public:
    Projector() : l0(1) { }
    Projector(double d) : l0(d) { }

    Vec3f operator()(const Vec3f& v) const
    {
        double s = 1.0 / (1.0 + v.z / l0);
        return Vec3f(s * v.x, s * v.y, s * v.z);
    }

    double get_l0() const
    {
        return l0;
    }
};

struct Color
{
    unsigned short R;
    unsigned short G;
    unsigned short B;

    Color();
    Color(unsigned short iR,
          unsigned short iG,
          unsigned short iB);

    Color operator*(const double& d) const
    {
        return Color(d * R, d * G, d * B);
    }

    Color operator+(const Color& c) const
    {
        return Color(R + c.R, G + c.G,  B + c.B);
    }

    Color operator-(const Color& c) const
    {
        return Color(R - c.R, G - c.G,  B - c.B);
    }

};

inline Color operator*(const double& d, const Color& c)
{
    return c * d;
}

struct Ray
{
    Vec3f beg;
    Vec3f dir;

    Ray();
    Ray(const Vec3f& b, const Vec3f& d);

    Vec3f operator()(double t) const
    {
        return beg + t * dir;
    }
};

enum { AMBIENT, POINT, PARALLEL };

struct LightSource
{
    int type;
    Vec3f position;
    double intensity;

    LightSource();
    LightSource(int t, const Vec3f& v, const double& intens);
};

struct Object
{
    Color color;
    double s;// приломление
    double r;// отражение
    virtual double is_intersect(const Ray& r) const = 0;
    virtual Vec3f get_normal(const Vec3f& v) const = 0;

    Object();
    Object(Color c);
    virtual void tick() { }
};

inline double is_intersect_tr(const Ray& ray,
                              const Vec3f& A,
                              const Vec3f& B,
                              const Vec3f& C)
{
    Vec3f AB = B - A;
    Vec3f AC = C - A;
    Vec3f AO = ray.beg - A;

    double delta = mixed(AB, AC, ray.dir);
    double u = mixed(AO, AC, ray.dir) / delta;
    double v = mixed(AB, AO, ray.dir) / delta;
    double t = -mixed(AB, AC, AO) / delta;

    if(u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1)
        return t;

    return -1.0;
}

struct Sphere : public Object
{

    double radius;
    Vec3f center;

    Vec3f dir;
    double step;
    double phase;
    double amp;
    // Новые поля для орбитального движения
    Vec3f orbitCenter;  // Центр орбиты
    double orbitRadius; // Радиус орбиты
    double angle;       // Текущий угол на орбите
    double angleStep;   // На сколько радиан поворачивать за 1 тик

    Sphere();
    Sphere(const double& rad, const Vec3f& v) ;
    Sphere(const double& rad, const Vec3f& v, Color c);
    Sphere(const double& rad, const Vec3f& v, Color c, double spect, double refl);

    Sphere(const double& rad, const Vec3f& v, Color c, double in_step, Vec3f in_dir, double in_amp);

    virtual Vec3f get_normal(const Vec3f& v) const override;

    Vec3f gep_pos() const;

    virtual double is_intersect(const Ray& r) const override;

    virtual void tick() override;

    void setOrbit(const Vec3f& orbitC, double radius, double initialAngle = 0.0, double stepAngle = 0.01);

};

struct Plane3v : public Object
{
    Vec3f vert[3];

    Plane3v();
    Plane3v(const Vec3f& A,
            const Vec3f& B,
            const Vec3f& C,
            Color col);
    Plane3v(const Vec3f& A,
            const Vec3f& B,
            const Vec3f& C,
            Color col, double spect, double refl);


    virtual double is_intersect(const Ray& r) const;

    virtual Vec3f get_normal(const Vec3f& v) const;
};

struct Plane4v : public Object
{
    Vec3f vert[4];

    Plane4v();
    Plane4v(const Vec3f& A,
            const Vec3f& B,
            const Vec3f& C,
            const Vec3f& D,
            Color col);
    Plane4v(const Vec3f& A,
            const Vec3f& B,
            const Vec3f& C,
            const Vec3f& D,
            Color col, double spect, double refl);


    virtual double is_intersect(const Ray& r) const;
    virtual Vec3f get_normal(const Vec3f& v) const;
};



#endif // OBJECTS_H
