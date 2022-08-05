#ifndef GRAPHICSLINES_H
#define GRAPHICSLINES_H

#include "graphicsitem.h"
#include <qopengltexture.h>
#include <lightproperty.h>
#include <materialproperty.h>
#include <QOpenGLShaderProgram>
class GraphicsLines : public GraphicsItem
{
    Q_OBJECT
public:
    GraphicsLines();
public:
    void draw(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    void updateBuffer();
    void appendPoint(QVector3D p);
    void clearRoute();
    void setRoute(QList<QVector3D> route);
protected:
    QList<QVector3D> route;
    static QOpenGLShaderProgram program;
};

#endif // GRAPHICSLINES_H
