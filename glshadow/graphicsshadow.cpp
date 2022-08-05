#include "graphicsshadow.h"
QOpenGLShaderProgram GraphicsShadow::program;
GraphicsShadow::GraphicsShadow()
{

}

void GraphicsShadow
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
    if(!texture)
    {
//        qDebug() << "shadow tex none";
//        return;
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
    QMatrix4x4 shadowScaleBias=QMatrix4x4(0.5,0.0,0.0,0.5,
                                          0.0,0.5,0.0,0.5,
                                          0.0,0.0,0.5,0.5,
                                          0.0,0.0,0.0,1.0);

    program.setUniformValue("qt_ShadowScaleBias",shadowScaleBias);

    GLuint position=program.attributeLocation("qt_Vertex");
    program.setAttributeBuffer(position,GL_FLOAT,0,3);
    program.enableAttributeArray(position);

    GLuint color=program.attributeLocation("qt_Color");
    program.setAttributeBuffer(color,GL_FLOAT,3*4*sizeof(GLfloat),3);
    program.enableAttributeArray(color);

    GLuint texcoord=program.attributeLocation("qt_TexCoord");
    program.setAttributeBuffer(texcoord,GL_FLOAT,
                               3*4*sizeof(GLfloat)+3*4*sizeof(GLfloat),2);
    program.enableAttributeArray(texcoord);

    GLuint normal=program.attributeLocation("qt_Normal");
    program.setAttributeBuffer(normal,GL_FLOAT,
                               3*4*sizeof(GLfloat)
                               +3*4*sizeof(GLfloat)
                               +2*4*sizeof(GLfloat),3);
    program.enableAttributeArray(normal);

//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D,dptexId);
//    program.setUniformValue("qt_DepthTexture",1);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
//                    GL_COMPARE_REF_TO_TEXTURE);

//    texture->bind(0);

//    program.setUniformValue("qt_DepthTexture",dptexId);

//    glDrawArrays(GL_LINES,0,trn);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    vBuffer.release();
    program.release();
}

void GraphicsShadow::updateBuffer()
{
    GLfloat vertices[3*4]={
        center.x()-w/2,center.y()-h/2, center.z(),
        center.x()+w/2,center.y()-h/2, center.z(),
        center.x()-w/2,center.y()+h/2, center.z(),
        center.x()+w/2,center.y()+h/2, center.z(),
    };
    GLfloat colors[3*4]={
        0.8,0.8,0.8,
        0.8,0.8,0.8,
        0.8,0.8,0.8,
        0.8,0.8,0.8,
    };
    GLfloat texCoords[2*4]={
        0.0,0.0,
        1.0,0.0,
        0.0,1.0,
        1.0,1.0,
    };
    GLfloat normals[3*4]={
        0.0,0.0,1.0,
        0.0,0.0,1.0,
        0.0,0.0,1.0,
        0.0,0.0,1.0,
    };
    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/floor.vsh"
                 << vshader->compileSourceFile(":/shader/floor.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/floor.fsh"
                 << fshader->compileSourceFile(":/shader/floor.fsh");

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
    }
    program.bind();

    vBuffer.create();
    vBuffer.bind();
    vBuffer.allocate(3*4*sizeof(GLfloat)
                     +3*4*sizeof(GLfloat)
                     +2*4*sizeof(GLfloat)
                     +3*4*sizeof(GLfloat));

    vBuffer.write(0,vertices,3*4*sizeof(GLfloat));
    vBuffer.write(3*4*sizeof(GLfloat),colors,3*4*sizeof(GLfloat));
    vBuffer.write(3*4*sizeof(GLfloat)+3*4*sizeof(GLfloat),
                  texCoords,2*4*sizeof(GLfloat));
    vBuffer.write(3*4*sizeof(GLfloat)+3*4*sizeof(GLfloat)+2*4*sizeof(GLfloat),
                  normals,3*4*sizeof(GLfloat));

    vBuffer.release();
    inited = true;
    program.release();
}

void GraphicsShadow::setCenter(float x, float y, float z)
{
    center.setX(x);
    center.setY(y);
    center.setZ(z);
    GLfloat vertices[3*4]={
        center.x()-w/2,center.y()-h/2, center.z(),
        center.x()+w/2,center.y()-h/2, center.z(),
        center.x()-w/2,center.y()+h/2, center.z(),
        center.x()+w/2,center.y()+h/2, center.z(),
    };
    GLfloat colors[3*4]={
        0.8,0.8,0.8,
        0.8,0.8,0.8,
        0.8,0.8,0.8,
        0.8,0.8,0.8,
    };
    GLfloat texCoords[2*4]={
        0.0,0.0,
        1.0,0.0,
        0.0,1.0,
        1.0,1.0,
    };
    GLfloat normals[3*4]={
        0.0,0.0,1.0,
        0.0,0.0,1.0,
        0.0,0.0,1.0,
        0.0,0.0,1.0,
    };
    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/floor.vsh"
                 << vshader->compileSourceFile(":/shader/floor.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/floor.fsh"
                 << fshader->compileSourceFile(":/shader/floor.fsh");

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
    }
    program.bind();

    if(!vBuffer.isCreated())
    {
        vBuffer.create();
        vBuffer.bind();
        vBuffer.allocate(3*4*sizeof(GLfloat)
                         +3*4*sizeof(GLfloat)
                         +2*4*sizeof(GLfloat)
                         +3*4*sizeof(GLfloat));
    }

    vBuffer.write(0,vertices,3*4*sizeof(GLfloat));
    vBuffer.write(3*4*sizeof(GLfloat),colors,3*4*sizeof(GLfloat));
    vBuffer.write(3*4*sizeof(GLfloat)+3*4*sizeof(GLfloat),
                  texCoords,2*4*sizeof(GLfloat));
    vBuffer.write(3*4*sizeof(GLfloat)+3*4*sizeof(GLfloat)+2*4*sizeof(GLfloat),
                  normals,3*4*sizeof(GLfloat));

    vBuffer.release();
    inited = true;
    program.release();
}

void GraphicsShadow::setLight(int index, LightProperty light)
{
    if(index>=MAX_LIGHT)
    {
        return;
    }
    QString si=QString::number(index);
    int loc;
    program.bind();
    loc=program.uniformLocation("qt_Lights["+si+"].isEnable");
    program.setUniformValue(loc,light.isEnable);

    loc=program.uniformLocation("qt_Lights["+si+"].isLocal");
    program.setUniformValue(loc,light.isLocal);

    loc=program.uniformLocation("qt_Lights["+si+"].isSpot");
    program.setUniformValue(loc,light.isSpot);

    loc=program.uniformLocation("qt_Lights["+si+"].ambient");
    program.setUniformValue(loc,light.ambient);

    loc=program.uniformLocation("qt_Lights["+si+"].color");
    program.setUniformValue(loc,light.color);

    loc=program.uniformLocation("qt_Lights["+si+"].position");
    program.setUniformValue(loc,light.position);

    loc=program.uniformLocation("qt_Lights["+si+"].halfVector");
    program.setUniformValue(loc,light.halfVector);

    loc=program.uniformLocation("qt_Lights["+si+"].focus");
    program.setUniformValue(loc,light.focus);

    loc=program.uniformLocation("qt_Lights["+si+"].spotCosCutoff");
    program.setUniformValue(loc,light.spotCosCutoff);

    loc=program.uniformLocation("qt_Lights["+si+"].spotExponent");
    program.setUniformValue(loc,light.spotExponent);

    loc=program.uniformLocation("qt_Lights["+si+"].constantAttenuation");
    program.setUniformValue(loc,light.constantAttenuation);

    loc=program.uniformLocation("qt_Lights["+si+"].linearAttenuation");
    program.setUniformValue(loc,light.linearAttenuation);

    loc=program.uniformLocation("qt_Lights["+si+"].quadraticAttenuation");
    program.setUniformValue(loc,light.quadraticAttenuation);

    loc=program.uniformLocation("qt_Lights["+si+"].projection");
    program.setUniformValue(loc,light.projection);

    loc=program.uniformLocation("qt_Lights["+si+"].view");
    program.setUniformValue(loc,light.view);

    loc=program.uniformLocation("qt_ShadowTextures["+si+"]");
    program.setUniformValue(loc,light.texIndex);

    glActiveTexture(GL_TEXTURE0+light.texIndex);
    glBindTexture(GL_TEXTURE_2D,light.texLoc);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    loc=program.uniformLocation("qt_ShadowTextureSizes["+si+"]");
    program.setUniformValue(loc,light.texSize);

    program.release();
}
