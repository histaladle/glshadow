#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
//#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
//#include <GL/glu.h>
#include <QMouseEvent>
#include <QPoint>
#include <qmath.h>
#include <QMatrix4x4>
#include "graphicsitem.h"
#include "graphicsorigin.h"
#include "graphicslines.h"
#include "graphicscube.h"
#include "graphicstriangles.h"
#include "stlloader.h"
#include "graphicsbillboard.h"
#include <QOpenGLFramebufferObject>
#include <QFileDialog>
#include <QOffscreenSurface>
#include "graphicsshadow.h"
#include <QQuaternion>
class QOpenGLShaderProgram;
class OGLWidget :
        public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OGLWidget(QWidget *parent=nullptr);
protected:
    void initializeGL();
    void paintGL();
//    void resizeGL(int w, int h);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    void setViewPort(int left, int right, int bottom, int top);
    void paintOrigin();
    void paintLines(QList<QVector3D> points);
    void paintLineSegments();
public:
    void clearRoutes();
    bool isBetween(QVector3D p1, QVector3D p2, QVector3D p3);
    void lookVertical();
private:
    float r = 10;
    QPoint mousePosLast;
    float pitch = 0.0;
    float yaw = 0.0;
    float roll = 0.0;
    float theta = 0.0;
    float phi = 0.0;

    float xRot = 0.0;
    float yRot = 0.0;
    float zRot = 0.0;
    /* glsl: m[col][row], m[2][3]: col:3,row:4 */
    QVector3D displace = QVector3D(0.0, 0.0, -3.0);
    QVector3D scale = QVector3D(1.0, 1.0, 1.0);
    QVector3D eye = QVector3D(0.0,0.0,0.0);
    /* eye focus on point */
    QVector3D focus = QVector3D(0.0,0.0,-3.0);
    QVector3D up = QVector3D(0.0,1.0,0.0);
    QOpenGLShaderProgram *program=nullptr;
    QOpenGLShaderProgram *billboardProgram=nullptr;
//    QOpenGLBuffer vbo;
    GraphicsOrigin *origin=nullptr;
    QOpenGLFramebufferObject *fbo=nullptr;
    QOpenGLFramebufferObject *fbos[MAX_LIGHT]={nullptr};
    QOpenGLFramebufferObject *idfbo=nullptr;

    LightProperty lights[MAX_LIGHT];

//    GLuint fboId,depthBufId,depthTexId;
protected:
    QList<GraphicsItem*> items;
    QList<GraphicsBillBoard*> boards;
    GraphicsShadow *shd;

    QImage img;
protected:
    QList<QList<QVector3D>> spaceRoutes;
public:
    void renderDepthFromLight(int index);
    void renderId();
    void saveFbo(QString filename);
signals:
private slots:
};

#endif // GLWIDGET_H
