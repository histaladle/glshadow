#ifndef GRAPHICSORIGIN_H
#define GRAPHICSORIGIN_H

#include "graphicsitem.h"
class GraphicsOrigin : public GraphicsItem
{
    Q_OBJECT
public:
    GraphicsOrigin();
public:
    void updateBuffer();
public:
    void draw(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
protected:
    static QOpenGLShaderProgram program;
};

#endif // GRAPHICSORIGIN_H
