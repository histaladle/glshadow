#ifndef GRAPHICSCUBE_H
#define GRAPHICSCUBE_H

#include <qopengltexture.h>
#include "graphicsitem.h"
#include "lightproperty.h"
#include "materialproperty.h"
#include <QCoreApplication>
class GraphicsCube : public GraphicsItem
{
    Q_OBJECT
public:
    GraphicsCube();
public:
    void draw(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    void updateBuffer();

    QOpenGLTexture *texture=nullptr;
//    QOpenGLBuffer indexBuf;
    void setLight(int index,LightProperty light);
    void renderDepth(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    void renderId(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
protected:
    static QOpenGLShaderProgram program;
    static QOpenGLShaderProgram depthShader;
    static QOpenGLShaderProgram idShader;
};

#endif // GRAPHICSCUBE_H
