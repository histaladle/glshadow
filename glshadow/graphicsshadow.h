#ifndef GRAPHICSSHADOW_H
#define GRAPHICSSHADOW_H

#include "graphicsitem.h"
#include <QOpenGLTexture>
class GraphicsShadow : public GraphicsItem
{
    Q_OBJECT
public:
    GraphicsShadow();
public:
    void draw(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
    void updateBuffer();
    void setShadow(QMatrix4x4 projection,QMatrix4x4 view,
                   QMatrix4x4 model,QImage image);
    void setShadow(QMatrix4x4 projection,QMatrix4x4 view,
                   QMatrix4x4 model,GLuint texid);
    void setShadow(QMatrix4x4 projection, QMatrix4x4 view,
                   QMatrix4x4 model, QOpenGLTexture *texture);
    void setShadowMap(GLuint texid);
    void setLight(int index, LightProperty light);
    void setCenter(float w,float h,float z);

    float w=200.0,h=200.0;
    QVector3D center;
private:
    static QOpenGLShaderProgram program;
    QOpenGLTexture *texture=nullptr;
//    GLuint dptexId;
    LightProperty lights[MAX_LIGHT];
};

#endif // GRAPHICSSHADOW_H
