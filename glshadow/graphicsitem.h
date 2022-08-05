#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QPoint>
#include <qmath.h>
#include "lightproperty.h"
class GraphicsItem : public QObject,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GraphicsItem();
    virtual ~GraphicsItem();
public:
    virtual void updateBuffer();
    virtual void draw(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    virtual void setLight(int index,LightProperty light);
    virtual void renderDepth(QMatrix4x4 projection,
                             QMatrix4x4 view,
                             QMatrix4x4 model);
    virtual void renderId(QMatrix4x4 projection,
                          QMatrix4x4 view,
                          QMatrix4x4 model);
protected:
    bool inited = false;
    QOpenGLBuffer vBuffer;
};

#endif // GRAPHICSITEM_H
