#ifndef GRAPHICSTRIANGLES_H
#define GRAPHICSTRIANGLES_H

#include "graphicsitem.h"
#include "facet.h"
#include "lightproperty.h"
#include "materialproperty.h"
class GraphicsTriangles : public GraphicsItem
{
    Q_OBJECT
public:
    GraphicsTriangles();
public:
    void draw(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    void updateBuffer();
    void setTriangles(QList<Facet> triangles);
    void centerTriangles();
    void setLight(int index,LightProperty light);
    void renderDepth(QMatrix4x4 projection,QMatrix4x4 view,QMatrix4x4 model);
    void renderId(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
    float getMinZ();
private:
    QList<Facet> triangles;
    static QOpenGLShaderProgram program;
    static QOpenGLShaderProgram depthShader;
    static QOpenGLShaderProgram idShader;
};

#endif // GRAPHICSTRIANGLES_H
