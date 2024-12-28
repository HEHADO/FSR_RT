#include "qraytracingwidget.h"
#include <QPainter>
#include <QImage>
#include <iostream>
#include <Qtimer>
#include <math.h>

QRayTracingWidget::QRayTracingWidget(QWidget *parent)
    : QWidget{parent}, cur_sc(width(), height(), width() + height())
{
    Vec3f Orbit_center(-50, 200, 250);
    double orbit_radius = 300;


    Sphere* s1 = new Sphere(30, Vec3f(-50, 200, 250), Color(93, 57, 84), 70, 0.3);
    Sphere* s2 = new Sphere(60, Vec3f(200, 200, 250 ), Color(123, 160, 91), 70, 0.3);
    Sphere* s3 = new Sphere(90, Vec3f(400, 200, 250 ), Color(220, 157, 0), 70, 0.3);


    cur_sc.add_object(s1);
    cur_sc.add_object(s2);
    cur_sc.add_object(s3);
    // Напоминалка про vec3f(лево-право, выше-ниже, ближе-дальше)
    Vec3f V1(-100, 0, 200), V2(-100, 0, 300), V3(0, 0, 300), V4(0, 0, 200);
    Vec3f V5(-100, -100, 200), V6(-100, -100, 300), V7(0, -100, 300), V8(0, -100, 200);
    //кууууубик
    cur_sc.add_object(new Plane4v(V1, V2, V3, V4,
                                  Color(255, 40, 195), -1, -1));
    cur_sc.add_object(new Plane4v(V1, V2, V6, V5,
                                  Color(255, 10, 100), -1, -1));
    cur_sc.add_object(new Plane4v(V2, V3, V7, V6,
                                  Color(255, 10, 100), -1, -1));
    cur_sc.add_object(new Plane4v(V3, V4, V8, V7,
                                  Color(255, 10, 100), -1, -1));
    cur_sc.add_object(new Plane4v(V5, V1, V4, V8,
                                  Color(255, 10, 100), -1, -1));

    // плоскость
    cur_sc.add_object(new Plane4v(Vec3f(-11100, -100, -1000),
                                  Vec3f(11100,  -100, -1000),
                                  Vec3f(11100,  -100, 8000),
                                  Vec3f(-11100, -100, 8000),
                                  Color(150, 255, 118), -1, -1));
    // плоскость(Зеркало)
    cur_sc.add_object(new Plane4v(Vec3f(-11100, 600, 6000),
                                  Vec3f(11100,  600, 6000),
                                  Vec3f(11100,  -600, 6000),
                                  Vec3f(-11100, -600, 6000),
                                  Color(240, 240, 240), 0, 1));

    cur_sc.add_light(LightSource(AMBIENT, Vec3f(0, 700, 0), 0.3));
    cur_sc.add_light(LightSource(POINT, Vec3f(700, 250, -500), 0.4));
    cur_sc.add_light(LightSource(POINT, Vec3f(-700, 250, -500), 0.4));

}


void QRayTracingWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int wW = width();
    int hW = height();




    cur_sc.resize(wW, hW);
    QImage img = cur_sc.render();
    p.drawImage(0, 0, img);

    QPen cpen(Qt::red);
    cpen.setWidth(3);
    QPen oldpen = p.pen();
    p.setPen(cpen);
    p.drawRect(0, 0, wW - 1, hW - 1) ;
    QString qs = QString :: asprintf("(%d,%d)", wW, hW);
    p.drawText(p.window().width() / 2, p.window().height() - 20, qs);
    p.setPen(oldpen);
}

void QRayTracingWidget::MakeFilm()
{



}
