#include "graphicsbillboard.h"

QOpenGLShaderProgram GraphicsBillBoard::program;
GraphicsBillBoard::GraphicsBillBoard()
{

}

void GraphicsBillBoard
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

    QMatrix4x4 normalMat;
    QMatrix4x4 mvMat=view*model;
    normalMat = mvMat.inverted().transposed();

    program.setUniformValue("qt_Model",model);
    program.setUniformValue("qt_View",view);
    program.setUniformValue("qt_Projection",projection);
    program.setUniformValue("qt_NormalMatrix",normalMat);

    GLuint position,texcoord;
    position=program.attributeLocation("qt_Vertex");
    program.setAttributeBuffer(position,GL_FLOAT,0,2,0);
    program.enableAttributeArray(position);

    texcoord=program.attributeLocation("qt_TexCoord");
    program.setAttributeBuffer(texcoord,GL_FLOAT,
                                4*2*sizeof(GLfloat),2,0);
    program.enableAttributeArray(texcoord);

    program.setUniformValue("qt_Center",center);

    program.setUniformValue("qt_Texture", 0);

    texture->bind(0);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    texture->release();
    program.release();
}

void GraphicsBillBoard::updateBuffer()
{
    texture=new QOpenGLTexture(QImage(":/image/side1.png").mirrored());
    GLfloat vertices[4*2] =
    {
        center.x()-width/2,center.y()+height/2,
        center.x()+width/2,center.y()+height/2,
        center.x()-width/2,center.y()-height/2,
        center.x()+width/2,center.y()-height/2,
    };
    GLfloat texcoords[4*2] =
    {
        0.0,1.0,
        1.0,1.0,
        0.0,0.0,
        1.0,0.0,
    };
    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/billboard.vsh"
                 << vshader->compileSourceFile(":/shader/billboard.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/billboard.fsh"
                 << fshader->compileSourceFile(":/shader/billboard.fsh");

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
    }
    program.bind();

    vBuffer.create();
    vBuffer.bind();
    vBuffer.allocate(4*2*sizeof(GLfloat)
                     +4*2*sizeof(GLfloat));

    vBuffer.bind();
    vBuffer.write(0, vertices, 4*2*sizeof(GLfloat));
    vBuffer.write(4*2*sizeof(GLfloat),texcoords,4*2*sizeof(GLfloat));

    inited=true;
    program.release();
}

void GraphicsBillBoard::setCenter(QVector3D pos)
{
    center=pos;
}

void GraphicsBillBoard::setTexture(QOpenGLTexture *texture)
{
    this->texture=texture;
}
