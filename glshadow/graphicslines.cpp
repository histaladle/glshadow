#include "graphicslines.h"
QOpenGLShaderProgram GraphicsLines::program;
GraphicsLines::GraphicsLines()
{

}

void GraphicsLines
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
    int pn;
    pn = route.size();
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
                               pn*3*sizeof(GLfloat), 3, 0);
    glEnableVertexAttribArray(color);

    glDrawArrays(GL_LINE_STRIP, 0, pn-1);

    vBuffer.release();
    program.release();
}

void GraphicsLines::updateBuffer()
{
    int pn = route.size();
    GLfloat lvertices[3];
    GLfloat lcolors[3];
    QVector3D p;

    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/line.vsh"
                 << vshader->compileSourceFile(":/shader/line.vsh");
//        vshader->compileSourceFile("./line.vsh");
        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/line.fsh"
                 << fshader->compileSourceFile(":/shader/line.fsh");
//        fshader->compileSourceFile("./line.fsh");
        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
    }

    program.bind();

    if(vBuffer.isCreated())
    {
        vBuffer.destroy();
    }
    vBuffer.create();
    vBuffer.bind();
    vBuffer.allocate(pn*2*3*sizeof(GLfloat));
//    qDebug() << "pn" << pn;
    for(int i=0; i<pn; i++)
    {
        p = route[i];

        lvertices[0]=p.x();
        lvertices[1]=p.y();
        lvertices[2]=p.z();

        lcolors[0]=0.5;
        lcolors[1]=0.5;
        lcolors[2]=0.5;

        vBuffer.write(i*3*sizeof(GLfloat), lvertices,
                  3*sizeof(GLfloat));
        vBuffer.write(pn*3*sizeof(GLfloat)+i*3*sizeof(GLfloat),
                  lcolors, 3*sizeof(GLfloat));
    }
//    qDebug() << "vbo buffer" << vBuffer.size();
    vBuffer.release();
    inited = true;
    program.release();
}

void GraphicsLines::appendPoint(QVector3D p)
{
    route.append(p);
    updateBuffer();
}

void GraphicsLines::setRoute(QList<QVector3D> route)
{
    this->route.clear();
    this->route = route;
    updateBuffer();
}

void GraphicsLines::clearRoute()
{
    route.clear();
    updateBuffer();
    inited = false;
}
