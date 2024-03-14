#include "qraytracingwidget.h"
#include <QPainter>
#include <QImage>
#include <iostream>

QRayTracingWidget::QRayTracingWidget(QWidget *parent)
    : QWidget{parent}, cur_sc(width(), height(), width() + height())
{
  cur_sc.add_object(new Sphere(130, Vec3f(300, 50, 200), Color(12, 100, 15), 20, 0.4));
  cur_sc.add_object(new Sphere(90, Vec3f(250, 170, 350), Color(100, 50, 107), 70, 0));
  cur_sc.add_object(new Sphere(30, Vec3f(100, 50, -100), Color(255, 0, 0), 20, 0.3));




    // Звездочка
    cur_sc.add_object(new Plane3v(Vec3f(100, 150, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(80, 210, -150),
                                 Color(239, 230, 230) ));
    cur_sc.add_object(new Plane3v(Vec3f(100, 150, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(120, 210, -150),
                                 Color(239, 230, 230) ));
    cur_sc.add_object(new Plane3v(Vec3f(100, 310, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(80, 250, -150),
                                 Color(239, 230, 230) ));
    cur_sc.add_object(new Plane3v(Vec3f(100, 310, -250),
                                 Vec3f(100, 210, -250),
                                 Vec3f(120, 250, -150),
                                 Color(239, 230, 239) ));
    cur_sc.add_object(new Plane3v(Vec3f(180, 230, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(120, 210, -150),
                                 Color(239, 230, 230) ));
    cur_sc.add_object(new Plane3v(Vec3f(180, 230, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(120, 250, -150),
                                 Color(239, 230, 230) ));
    cur_sc.add_object(new Plane3v(Vec3f(20, 230, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(80, 210, -150),
                                 Color(239, 230, 230) ));
    cur_sc.add_object(new Plane3v(Vec3f(20, 230, -250),
                                 Vec3f(100, 230, -250),
                                 Vec3f(80, 250, -150),
                                 Color(239, 230, 230) ));
//new

    cur_sc.add_object(new Plane4v(Vec3f(-500, -150, -200),
                                  Vec3f(-500,  -350, -200),
                                  Vec3f(-200,  -350, -500),
                                  Vec3f(-200, -150, -500),
                                  Color(50, 15, 80), 20, 0.4));

    cur_sc.add_object(new Plane4v(Vec3f(100, -150, -200),
                                  Vec3f(100,  -350, -200),
                                  Vec3f(-200,  -350, -500),
                                  Vec3f(-200, -150, -500),
                                  Color(100, 100, 80), 20, 0.4));

    cur_sc.add_object(new Plane3v(Vec3f(-500, -150, -200),
                                  Vec3f(-200, -150, -500),
                                  Vec3f(100, -150, -200),
                                  Color(239, 230, 230), 20, 0.4 ));
//    //new object
//    cur_sc.add_object(new Plane3v(Vec3f(150, 60, 250),
//                                  Vec3f(500, 60, 230),
//                                  Vec3f(230, 60, 700),
//                                  Color(239, 230, 230) ));

  // плоскость
  cur_sc.add_object(new Plane4v(Vec3f(-1100, -600, 100),
                                Vec3f(1100,  -600, 100),
                                Vec3f(1100,  -600, 8000),
                                Vec3f(-1100, -600, 8000),
                                Color(150, 72, 118), -1, -1));
  // плоскость
  cur_sc.add_object(new Plane4v(Vec3f(-1100, 600, 6000),
                                Vec3f(1100,  600, 6000),
                                Vec3f(1100,  -600, 6000),
                                Vec3f(-1100, -600, 6000),
                                Color(240, 240, 240), 0, 1));



  // источники света
  cur_sc.add_light(LightSource(AMBIENT, Vec3f(), 0.3));
  cur_sc.add_light(LightSource(POINT, Vec3f(100, 200, 0), 0.8));
  cur_sc.add_light(LightSource(POINT, Vec3f(-700, 250, -1000), 0.1));
}

const int chess_cell_size = 30;

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
  int num_frames = 100;
  for(int nf = 0; nf < num_frames; nf++)
  {
    cur_sc.tick();
    QImage img = cur_sc.render();
    QString fname_img = QString::asprintf("img_%03d.png", nf);
    img.save(fname_img);
    std :: cerr << "Pict " << fname_img.toStdString() << " was output" << std :: endl;
  }
  system("mencoder \"mf:\/\/\*.png\" -vf scale=1400:-11 -oac copy -ovc lavc -lavcopts vcodec=msmpeg4:vbitrate=2000 -ffourcc MP43 -o \"output2.avi\"");
}
