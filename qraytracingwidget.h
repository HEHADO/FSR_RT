#ifndef QRAYTRACINGWIDGET_H
#define QRAYTRACINGWIDGET_H

#include <QWidget>
#include "scene.h"

class QRayTracingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRayTracingWidget(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *);

    Scene cur_sc;

    void MakeFilm();

signals:


};

#endif // QRAYTRACINGWIDGET_H
