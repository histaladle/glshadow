#include "graphicstriangles.h"
QOpenGLShaderProgram GraphicsTriangles::program;
QOpenGLShaderProgram GraphicsTriangles::depthShader;
QOpenGLShaderProgram GraphicsTriangles::idShader;
GraphicsTriangles::GraphicsTriangles()
{

}

void GraphicsTriangles
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
    QMatrix4x4 shadowScaleBias=QMatrix4x4(0.5,0.0,0.0,0.5,
                                          0.0,0.5,0.0,0.5,
                                          0.0,0.0,0.5,0.5,
                                          0.0,0.0,0.0,1.0);

    program.setUniformValue("qt_ShadowScaleBias",shadowScaleBias);

    int trn=triangles.size();

    GLuint position=program.attributeLocation("qt_Vertex");
    program.setAttributeBuffer(position,GL_FLOAT,0,3);
    program.enableAttributeArray(position);

    GLuint color=program.attributeLocation("qt_Color");
    program.setAttributeBuffer(color,GL_FLOAT,3*3*trn*sizeof(GLfloat),3);
    program.enableAttributeArray(color);

    GLuint normal=program.attributeLocation("qt_Normal");
    program.setAttributeBuffer(normal,GL_FLOAT,
                                3*3*trn*sizeof(GLfloat)
                                +3*3*trn*sizeof(GLfloat),3);
    program.enableAttributeArray(normal);

//    glDrawArrays(GL_LINES,0,trn);
    glDrawArrays(GL_TRIANGLES,0,trn*3);
    for(int i=0;i<trn;i++)
    {
//        glDrawArrays(GL_TRIANGLES,i*3,3);
//        glDrawArrays(GL_LINE_LOOP,i*3,3);
    }
    vBuffer.release();
    program.release();
}

void GraphicsTriangles::updateBuffer()
{
    int trn=triangles.size();
    GLfloat *trvertices=new GLfloat[3*3*trn];
    GLfloat *colors=new GLfloat[3*3*trn];
    GLfloat *normals=new GLfloat[3*3*trn];
    for(int i=0;i<trn;i++)
    {
        trvertices[3*3*i+0]=triangles[i].v[0].x();
        trvertices[3*3*i+1]=triangles[i].v[0].y();
        trvertices[3*3*i+2]=triangles[i].v[0].z();

        trvertices[3*3*i+3]=triangles[i].v[1].x();
        trvertices[3*3*i+4]=triangles[i].v[1].y();
        trvertices[3*3*i+5]=triangles[i].v[1].z();

        trvertices[3*3*i+6]=triangles[i].v[2].x();
        trvertices[3*3*i+7]=triangles[i].v[2].y();
        trvertices[3*3*i+8]=triangles[i].v[2].z();

        colors[3*3*i+0]=1.0;
        colors[3*3*i+1]=0.5;
        colors[3*3*i+2]=0.5;

        colors[3*3*i+3]=1.0;
        colors[3*3*i+4]=0.5;
        colors[3*3*i+5]=0.5;

        colors[3*3*i+6]=1.0;
        colors[3*3*i+7]=0.5;
        colors[3*3*i+8]=0.5;

        normals[3*3*i+0]=triangles[i].normal.x();
        normals[3*3*i+1]=triangles[i].normal.y();
        normals[3*3*i+2]=triangles[i].normal.z();

        normals[3*3*i+3]=triangles[i].normal.x();
        normals[3*3*i+4]=triangles[i].normal.y();
        normals[3*3*i+5]=triangles[i].normal.z();

        normals[3*3*i+6]=triangles[i].normal.x();
        normals[3*3*i+7]=triangles[i].normal.y();
        normals[3*3*i+8]=triangles[i].normal.z();
    }
    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/facet.vsh"
                 << vshader->compileSourceFile(":/shader/facet.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/facet.fsh"
                 << fshader->compileSourceFile(":/shader/facet.fsh");

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
    }
    program.bind();

    MaterialProperty material;
    material.emission=QVector3D(0.0,0.0,0.0);
    material.ambient=QVector3D(0.8,0.8,0.8);
    material.diffuse=QVector3D(0.8,0.8,0.8);
    material.specular=QVector3D(1.0,1.0,1.0);
    material.shininess=100.0;

    program.setUniformValue("Material.emission",
                             material.emission);
    program.setUniformValue("Material.ambient",
                             material.ambient);
    program.setUniformValue("Material.diffuse",
                             material.diffuse);
    program.setUniformValue("Material.specular",
                             material.specular);
    program.setUniformValue("Material.shininess",
                             material.shininess);

    program.setUniformValue("qt_Strength",(GLfloat)0.8);

    vBuffer.create();
    vBuffer.bind();
    vBuffer.allocate(3*3*trn*sizeof(GLfloat)
                     +3*3*trn*sizeof(GLfloat)
                     +3*3*trn*sizeof(GLfloat));

    vBuffer.write(0,trvertices,3*3*trn*sizeof(GLfloat));
    vBuffer.write(3*3*trn*sizeof(GLfloat),colors,3*3*trn*sizeof(GLfloat));
    vBuffer.write(3*3*trn*sizeof(GLfloat)+3*3*trn*sizeof(GLfloat),
                  normals,3*3*trn*sizeof(GLfloat));
    qDebug() << "vbo buffer" << vBuffer.size();
    vBuffer.release();
    inited = true;

    delete trvertices;
    delete colors;
    delete normals;

    program.release();

    if(!depthShader.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/facet.vsh"
                 << vshader->compileSourceFile(":/shader/facet.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/depth.fsh"
                 << fshader->compileSourceFile(":/shader/depth.fsh");

        depthShader.addShader(vshader);
        depthShader.addShader(fshader);
        depthShader.link();
    }

    if(!idShader.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/facet.vsh"
                 << vshader->compileSourceFile(":/shader/facet.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/id.fsh"
                 << fshader->compileSourceFile(":/shader/id.fsh");

        idShader.addShader(vshader);
        idShader.addShader(fshader);
        idShader.link();
    }
    idShader.bind();
    idShader.setUniformValue("qt_ObjectId",9);
    idShader.release();
}

void GraphicsTriangles
::renderId(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
{
    if(!vBuffer.isCreated())
    {
        return;
    }
    if(!inited)
    {
        return;
    }
    idShader.bind();
    vBuffer.bind();

    QMatrix4x4 normalMat;
    QMatrix4x4 mvMat=view*model;
    normalMat = mvMat.inverted().transposed();

    idShader.setUniformValue("qt_Model",model);
    idShader.setUniformValue("qt_View",view);
    idShader.setUniformValue("qt_Projection",projection);
    idShader.setUniformValue("qt_NormalMatrix",normalMat);

    int trn=triangles.size();

    GLuint position=idShader.attributeLocation("qt_Vertex");
    idShader.setAttributeBuffer(position,GL_FLOAT,0,3);
    idShader.enableAttributeArray(position);

    GLuint color=idShader.attributeLocation("qt_Color");
    idShader.setAttributeBuffer(color,GL_FLOAT,3*3*trn*sizeof(GLfloat),3);
    idShader.enableAttributeArray(color);

    GLuint normal=idShader.attributeLocation("qt_Normal");
    idShader.setAttributeBuffer(normal,GL_FLOAT,
                                3*3*trn*sizeof(GLfloat)
                                +3*3*trn*sizeof(GLfloat),3);
    idShader.enableAttributeArray(normal);

//    glDrawArrays(GL_LINES,0,trn);
    glDrawArrays(GL_TRIANGLES,0,trn*3);
    for(int i=0;i<trn;i++)
    {
//        glDrawArrays(GL_TRIANGLES,i*3,3);
//        glDrawArrays(GL_LINE_LOOP,i*3,3);
    }
    vBuffer.release();
    idShader.release();
}

void GraphicsTriangles
::renderDepth(QMatrix4x4 projection,QMatrix4x4 view, QMatrix4x4 model)
{
    if(!vBuffer.isCreated())
    {
        return;
    }
    if(!inited)
    {
        return;
    }
    depthShader.bind();
    vBuffer.bind();

    QMatrix4x4 normalMat;
    QMatrix4x4 mvMat=view*model;
    normalMat = mvMat.inverted().transposed();

    depthShader.setUniformValue("qt_Model",model);
    depthShader.setUniformValue("qt_View",view);
    depthShader.setUniformValue("qt_Projection",projection);
    depthShader.setUniformValue("qt_NormalMatrix",normalMat);

    int trn=triangles.size();

    GLuint position=depthShader.attributeLocation("qt_Vertex");
    depthShader.setAttributeBuffer(position,GL_FLOAT,0,3);
    depthShader.enableAttributeArray(position);

    GLuint color=depthShader.attributeLocation("qt_Color");
    depthShader.setAttributeBuffer(color,GL_FLOAT,3*3*trn*sizeof(GLfloat),3);
    depthShader.enableAttributeArray(color);

    GLuint normal=depthShader.attributeLocation("qt_Normal");
    depthShader.setAttributeBuffer(normal,GL_FLOAT,
                                3*3*trn*sizeof(GLfloat)
                                +3*3*trn*sizeof(GLfloat),3);
    depthShader.enableAttributeArray(normal);

//    glDrawArrays(GL_LINES,0,trn);
    glDrawArrays(GL_TRIANGLES,0,trn*3);
    for(int i=0;i<trn;i++)
    {
//        glDrawArrays(GL_TRIANGLES,i*3,3);
//        glDrawArrays(GL_LINE_LOOP,i*3,3);
    }
    vBuffer.release();
    depthShader.release();
}

void GraphicsTriangles::setTriangles(QList<Facet> triangles)
{
    qDebug() << "in triangles num" << triangles.size();
    this->triangles=triangles;
    inited=false;
}

void GraphicsTriangles::centerTriangles()
{
    int trn=this->triangles.size();
    QVector3D center=QVector3D(0.0,0.0,0.0);
    if(trn<=0)
    {
        return;
    }
    for(int i=0;i<trn;i++)
    {
        center+=triangles[i].v[0];
        center+=triangles[i].v[1];
        center+=triangles[i].v[2];
    }
    center/=trn*3;
    for(int i=0;i<trn;i++)
    {
        triangles[i].v[0]-=center;
        triangles[i].v[1]-=center;
        triangles[i].v[2]-=center;
    }
}

void GraphicsTriangles::setLight(int index, LightProperty light)
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

float GraphicsTriangles::getMinZ()
{
    float minz=0.0;
    for(int i=0;i<triangles.size();i++)
    {
        for(int j=0;j<3;j++)
        {
            if(minz>triangles[i].v[j].z())
            {
                minz=triangles[i].v[j].z();
            }
        }
    }
    return minz;
}
