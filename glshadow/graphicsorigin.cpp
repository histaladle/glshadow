#include "graphicsorigin.h"
QOpenGLShaderProgram GraphicsOrigin::program;
GraphicsOrigin::GraphicsOrigin() : GraphicsItem()
{
}

void GraphicsOrigin
::draw(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
{
    if(!vBuffer.isCreated())
    {
        return;
    }
    if(!inited)
    {
        return;
    }
    program.bind();
    vBuffer.bind();

    program.setUniformValue("qt_Model",model);
    program.setUniformValue("qt_View",view);
    program.setUniformValue("qt_Projection",projection);

    GLuint position, color;
    position = program.attributeLocation("qt_Vertex");
    program.setAttributeBuffer(position, GL_FLOAT, 0, 3, 0);
    glEnableVertexAttribArray(position);

    color = program.attributeLocation("qt_Color");
    program.setAttributeBuffer(color, GL_FLOAT,
                                18*sizeof(GLfloat), 3, 0);
    glEnableVertexAttribArray(color);

    glDrawArrays(GL_LINES, 0, 6);

    vBuffer.release();
    program.release();
}

void GraphicsOrigin::updateBuffer()
{
    QVector3D vx(5.0, 0.0, 0.0);
    QVector3D vy(0.0, 5.0, 0.0);
    QVector3D vz(0.0, 0.0, 5.0);
    QVector3D o;
    o = QVector3D(0.0, 0.0, 0.0);

    GLfloat lcolors[3][2][3] =
    {
        { {1.0f, 0.0f, 0.0f},
          {1.0f, 0.0f, 0.0f} },
        { {0.0f, 1.0f, 0.0f},
          {0.0f, 1.0f, 0.0f} },
        { {0.0f, 0.0f, 1.0f},
          {0.0f, 0.0f, 1.0f} }
    };

    GLfloat lvertices[3][2][3] =
    {
        { {o.x(), o.y(), o.z()},
          {vx.x(), vx.y(), vx.z()}, },
        { {o.x(), o.y(), o.z()},
          {vy.x(), vy.y(), vy.z()}, },
        { {o.x(), o.y(), o.z()},
          {vz.x(), vz.y(), vz.z()}, },
    };

    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/line.vsh"
                 << vshader->compileSourceFile(":/shader/line.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment,this);
        qDebug() << ":/shader/line.fsh"
                 << fshader->compileSourceFile(":/shader/line.fsh");

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
    }
    program.bind();

    vBuffer.create();
    vBuffer.bind();
    vBuffer.allocate(36*sizeof(GLfloat));
    vBuffer.write(0, lvertices, 18*sizeof(GLfloat));
    vBuffer.write(18*sizeof(GLfloat), lcolors, 18*sizeof(GLfloat));
    vBuffer.release();
    inited = true;

    program.release();
}
