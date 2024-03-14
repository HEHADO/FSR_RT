#ifndef SCENE_H
#define SCENE_H

#include <QImage>
#include <vector>
#include "geom.h"


class Scene
{

protected:

  int width;
  int height;
  Projector proj;

  std :: vector<Object *> objects;
  std :: vector<LightSource> lights;

public:
    Scene(double w = 100, double h = 100, double l0 = 200);
    QImage render();

    Color compLight(const Vec3f pt,const  size_t i);
    Color TraceR(Ray r, int rec_d);

    void clear() { objects.clear(); }
    void add_object(Object * o) { objects.push_back(o); }
    void add_light(const LightSource& s) { lights.push_back(s); }
    void resize(int w, int h)
    {
      width = w;
      height = h;
      proj = Projector(w + h);
    }

    void tick()
    {
      for(size_t q(0); q < objects.size(); q++)
        objects[q]->tick();
    }
};

#endif // SCENE_H
