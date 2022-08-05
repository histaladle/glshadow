#ifndef LIGHTPROPERTY_H
#define LIGHTPROPERTY_H

#include <QVector3D>
#include <QOpenGLFunctions>
const int MAX_LIGHT=3;
struct LightProperty {
    GLboolean isEnable;
    GLboolean isLocal;
    GLboolean isSpot;
    QVector3D ambient;
    QVector3D color;
    QVector3D position;
    QVector3D halfVector;
    QVector3D focus;
    GLfloat spotCosCutoff;
    GLfloat spotExponent;
    GLfloat constantAttenuation;
    GLfloat linearAttenuation;
    GLfloat quadraticAttenuation;

    QMatrix4x4 projection;
    QMatrix4x4 view;

    GLuint texIndex;
    GLuint texLoc;
    QVector2D texSize;
};

#endif // LIGHTPROPERTY_H
