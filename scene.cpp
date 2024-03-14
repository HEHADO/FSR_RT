#include "scene.h" // включаем заголовочный файл scene.h

Scene::Scene(double w, double h, double l0)
    : width(w), height(h), proj(l0) // конструктор класса Scene с инициализацией переменных width, height и proj
{

}
QImage Scene::render() // метод render класса Scene
{
    QImage ret(width, height, QImage::Format_RGB888); // создание изображения ret размером width x height

    Color amb(150, 150, 150); // создаем объект типа Color для подсветки сцены
    Vec3f org(-100, 300, -proj.get_l0()); // инициализация org - начальной точки луча

    for(int h = 0; h < height; h++) // цикл по высоте изображения
        for(int w = 0; w < width; w++) // цикл по ширине изображения
        {
            double X = w - width / 2; // вычисляем координату X пикселя
            double Y = height / 2 - h; // вычисляем координату Y пикселя

            Vec3f scr_pt(X, Y, 0); // координаты пикселя на экране
            Ray r(org, scr_pt - org); // создание луча от org до scr_pt

            double max_t = std::numeric_limits<double>::max(); // задаем максимальное значение параметра t
            Color cur_col(0, 0, 0); // инициализация цвета текущего пикселя
            cur_col = TraceR(r, 2); // трассировка луча и вычисление цвета
            ret.setPixel(w, h, qRgb(cur_col.R, cur_col.G, cur_col.B)); // установка цвета пикселя в изображении
        }

    return  ret; // возвращаем изображение
}

Color Scene::compLight(Vec3f pt, const size_t i){ // метод вычисления цвета в точке после взаимодействия с источниками света

    Color cur_col = Color(0,0,0); // инициализация текущего цвета как черного

    for(size_t l(0); l < lights.size(); l++)  { // цикл по всем источникам света
        LightSource& ls(lights[l]); // текущий источник света
        if (ls.type == AMBIENT){ // если тип источника - AMBIENT
            cur_col = cur_col + ls.intensity * objects[i]->color; // добавляем интенсивность подсветки от источника AMBIENT
        }
        else if(ls.type == POINT){ // если тип источника - POINT
            Vec3f un_ldir = ( ls.position - pt); // вектор от точки до источника
            Vec3f ldir = ( ls.position - pt) * (1 / (pt - ls.position).length()); // нормализация вектора направления к источнику

            double cos = (ldir * objects[i]->get_normal(pt)); // косинус угла между направлением света и нормалью
            if(cos >= 0)
                cur_col = cur_col + ls.intensity * cos * objects[i]->color; // добавляем освещение

            if(objects[i]->s != -1){ // если у объекта есть коэффициент блеска
                Vec3f R = (2 * objects[i]->get_normal(pt) * (ldir * objects[i]->get_normal(pt)) - ldir); // вектор отражения
                double r_dot_v = R.norm() * pt.norm(); // скалярное произведение вектора отражения и наблюдателя
                if (r_dot_v < 0)
                    cur_col = cur_col + ls.intensity * pow(r_dot_v, objects[i]->s) * objects[i]->color; // добавляем блик
            }
        }
    }
    return cur_col; // возвращаем текущий цвет
}

Color Scene::TraceR(Ray r, int rec_d){ // метод трассировки луча

    double max_t = std::numeric_limits<double>::max(); // задаем максимальное значение параметра t
    Color cur_col(0, 0, 0); // инициализация текущего цвета как черного

    size_t ind = -1; // индекс объекта сцены
    Vec3f pt; // точка пересечения
    Vec3f pt2; // точка пересечения для рефлексии
    double cur_t = -1; // параметр t для текущего объекта

    for(size_t i = 0; i < objects.size(); i++) // цикл по всем объектам сцены
    {
        cur_t = objects[i]->is_intersect(r); // проверка пересечения луча с объектом
        pt = r(cur_t); // вычисление точки пересечения
        if(cur_t > 0.0001) // если пересечение есть
        {
            if(cur_t < max_t) // если новое пересечение ближе
            {
                pt2 = pt; // запоминаем точку пересечения для рефлексии
                cur_col = compLight(pt, i); // вычисляем цвет с учетом источников света
                ind = i; // запоминаем индекс объекта
                max_t = cur_t; // обновляем параметр t
            }
        }
    }

    if(ind == -1) // если не было пересечений
        return cur_col; // возвращаем текущий цвет
    else {
        double rr = objects[ind]->r; // коэффициент отражения объекта
        if(rec_d <= 0 || rr == -1) // если достигнута максимальная глубина трассировки или объект не отражает свет
            return cur_col; // возвращаем текущий цвет

        if(max_t > 0.0001 && max_t != std::numeric_limits<double>::max()) // если было пересечение
        {
            Vec3f N = objects[ind]->get_normal(pt2); // нормаль к объекту в точке пересечения
            Vec3f Rq = (2 * N * ((-1 * r.dir) * N) + r.dir).norm(); // вектор отражения

            Color reflected_c = TraceR(Ray(pt2, Rq), rec_d - 1); // рекурсивно трассируем луч для отраженного света

            return cur_col * (1) + reflected_c * rr; // возвращаем сумму цветов
        }
        else {
            return cur_col; // возвращаем текущий цвет
        }
    }
}
