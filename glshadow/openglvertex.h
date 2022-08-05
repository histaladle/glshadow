#ifndef OPENGLVERTEX_H
#define OPENGLVERTEX_H

#include <QVector3D>
#include <QVector2D>
class OpenGLVertex
{
public:
    OpenGLVertex();
    QVector3D pos;
    QVector3D normal;
    QVector2D texCoord;
    bool operator ==(const OpenGLVertex other) const;
};

#endif // STRIPPOINT_H
