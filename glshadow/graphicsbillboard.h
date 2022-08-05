#ifndef GRAPHICSBILLBOARD_H
#define GRAPHICSBILLBOARD_H

#include "graphicsitem.h"
#include <QOpenGLTexture>
#include "lightproperty.h"
#include "materialproperty.h"

class GraphicsBillBoard : public GraphicsItem
{
    Q_OBJECT
public:
    GraphicsBillBoard();
    void draw(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    void updateBuffer();
    void setCenter(QVector3D pos);
    void setTexture(QOpenGLTexture *texture);
public:
private:
    QOpenGLTexture *texture=0;
    QVector3D center;
    float width=1.0,height=1.0;
    static QOpenGLShaderProgram program;
};

#endif // GRAPHICSBILLBOARD_H
