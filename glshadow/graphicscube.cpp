#include "graphicscube.h"
QOpenGLShaderProgram GraphicsCube::program;
QOpenGLShaderProgram GraphicsCube::depthShader;
QOpenGLShaderProgram GraphicsCube::idShader;
GraphicsCube::GraphicsCube()// : indexBuf(QOpenGLBuffer::IndexBuffer)
{

}

void GraphicsCube
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

//    indexBuf.bind();

    GLuint position, texcoord, normal;
    position = program.attributeLocation("qt_Vertex");
    program.setAttributeBuffer(position, GL_FLOAT, 0, 3, 0);
    program.enableAttributeArray(position);

    texcoord = program.attributeLocation("qt_TexCoord");
    program.setAttributeBuffer(texcoord, GL_FLOAT,
                               6*4*3*sizeof(GLfloat), 2, 0);
    program.enableAttributeArray(texcoord);

    normal = program.attributeLocation("qt_Normal");
    program.setAttributeBuffer(normal, GL_FLOAT,
                               6*4*3*sizeof(GLfloat)
                               +6*4*2*sizeof(GLfloat), 3, 0);
    program.enableAttributeArray(normal);


    program.setUniformValue("qt_Texture", 0);

    texture->bind(0);

//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    for(int i=0; i<6; i++)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
    }
//    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, (void*)0);
    texture->release();
    vBuffer.release();
    program.release();
}

void GraphicsCube::updateBuffer()
{
    texture = new QOpenGLTexture(QImage(":/image/cube.png").mirrored());
    GLfloat vertices[6][4][3] =
    {
        //face 0
        { {-1.0f, -1.0f,  1.0f},
          { 1.0f, -1.0f,  1.0f},
          {-1.0f,  1.0f,  1.0f},
          { 1.0f,  1.0f,  1.0f},},
        //face 1
        { { 1.0f, -1.0f,  1.0f},
          { 1.0f, -1.0f, -1.0f},
          { 1.0f,  1.0f,  1.0f},
          { 1.0f,  1.0f, -1.0f},},
        //face 2
        { { 1.0f, -1.0f, -1.0f},
          {-1.0f, -1.0f, -1.0f},
          { 1.0f,  1.0f, -1.0f},
          {-1.0f,  1.0f, -1.0f},},
        //face 3
        { {-1.0f, -1.0f, -1.0f},
          {-1.0f, -1.0f,  1.0f},
          {-1.0f,  1.0f, -1.0f},
          {-1.0f,  1.0f,  1.0f},},
        //face 4
        { {-1.0f, -1.0f, -1.0f},
          { 1.0f, -1.0f, -1.0f},
          {-1.0f, -1.0f,  1.0f},
          { 1.0f, -1.0f,  1.0f},},
        //face 5
        { {-1.0f,  1.0f,  1.0f},
          { 1.0f,  1.0f,  1.0f},
          {-1.0f,  1.0f, -1.0f},
          { 1.0f,  1.0f, -1.0f},},
    };
    GLfloat texcoords[6][4][2] =
    {
        //face 0
        { { 0.0f, 0.0f},
          {0.33f, 0.0f},
          { 0.0f, 0.5f},
          {0.33f, 0.5f},
          },
        //face 1
        { { 0.0f, 0.5f},
          {0.33f, 0.5f},
          { 0.0f, 1.0f},
          {0.33f, 1.0f},},
        //face 2
        { {0.66f, 0.5f},
          { 1.0f, 0.5f},
          {0.66f, 1.0f},
          { 1.0f, 1.0f},},
        //face 3
        { {0.66f, 0.0f},
          { 1.0f, 0.0f},
          {0.66f, 0.5f},
          { 1.0f, 0.5f},},
        //face 4
        { {0.33f, 0.0f},
          {0.66f, 0.0f},
          {0.33f, 0.5f},
          {0.66f, 0.5f},},
        //face 5
        { {0.33f, 0.5f},
          {0.66f, 0.5f},
          {0.33f, 1.0f},
          {0.66f, 1.0f},},
    };
    GLfloat normals[6][4][3] =
    {
        //face 0
        { { 0.0f,  0.0f,  1.0f},
          { 0.0f,  0.0f,  1.0f},
          { 0.0f,  0.0f,  1.0f},
          { 0.0f,  0.0f,  1.0f},},
        //face 1
        { { 1.0f,  0.0f,  0.0f},
          { 1.0f,  0.0f,  0.0f},
          { 1.0f,  0.0f,  0.0f},
          { 1.0f,  0.0f,  0.0f},},
        //face 2
        { { 0.0f,  0.0f, -1.0f},
          { 0.0f,  0.0f, -1.0f},
          { 0.0f,  0.0f, -1.0f},
          { 0.0f,  0.0f, -1.0f},},
        //face 3
        { {-1.0f,  0.0f, 0.0f},
          {-1.0f,  0.0f, 0.0f},
          {-1.0f,  0.0f, 0.0f},
          {-1.0f,  0.0f, 0.0f},},
        //face 4
        { { 0.0f, -1.0f, 0.0f},
          { 0.0f, -1.0f, 0.0f},
          { 0.0f, -1.0f, 0.0f},
          { 0.0f, -1.0f, 0.0f},},
        //face 5
        { { 0.0f,  1.0f, 0.0f},
          { 0.0f,  1.0f, 0.0f},
          { 0.0f,  1.0f, 0.0f},
          { 0.0f,  1.0f, 0.0f},},
    };

    initializeOpenGLFunctions();
    if(!program.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/texture.vsh"
                 << vshader->compileSourceFile(":/shader/texture.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/texture.fsh"
                 << fshader->compileSourceFile(":/shader/texture.fsh");

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
    vBuffer.allocate(6*4*3*sizeof(GLfloat)
                     +6*4*2*sizeof(GLfloat)
                     +6*4*3*sizeof(GLfloat));

    vBuffer.write(0, vertices, 6*4*3*sizeof(GLfloat));
    vBuffer.write(6*4*3*sizeof(GLfloat), texcoords,
                  6*4*2*sizeof(GLfloat));
    vBuffer.write(6*4*3*sizeof(GLfloat)+6*4*2*sizeof(GLfloat),
                  normals, 6*4*3*sizeof(GLfloat));

    vBuffer.release();

//    GLushort indices[] = {
//         0,  1,  2,  3,  3,
//         4,  4,  5,  6,  7,  7,
//         8,  8,  9, 10, 11, 11,
//        12, 12, 13, 14, 15, 15,
//        16, 16, 17, 18, 19, 19,
//        20, 20, 21, 22, 23
//    };
//    indexBuf.create();
//    indexBuf.bind();
//    indexBuf.allocate(indices, 34 * sizeof(GLushort));

    inited = true;
    program.release();
    if(!depthShader.isLinked())
    {
        QOpenGLShader *vshader=nullptr;
        vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
        qDebug() << ":/shader/texture.vsh"
                 << vshader->compileSourceFile(":/shader/texture.vsh");

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
                 << vshader->compileSourceFile("./facet.vsh");

        QOpenGLShader *fshader=nullptr;
        fshader=new QOpenGLShader(QOpenGLShader::Fragment, this);
        qDebug() << ":/shader/id.fsh"
                 << fshader->compileSourceFile(":/shader/id.fsh");

        idShader.addShader(vshader);
        idShader.addShader(fshader);
        idShader.link();
    }
    idShader.bind();
    idShader.setUniformValue("qt_ObjectId",19);
    idShader.release();
}

void GraphicsCube
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

//    indexBuf.bind();

    GLuint position, texcoord, normal;
    position = idShader.attributeLocation("qt_Vertex");
    idShader.setAttributeBuffer(position, GL_FLOAT, 0, 3, 0);
    idShader.enableAttributeArray(position);

    texcoord = idShader.attributeLocation("qt_TexCoord");
    idShader.setAttributeBuffer(texcoord, GL_FLOAT,
                               6*4*3*sizeof(GLfloat), 2, 0);
    idShader.enableAttributeArray(texcoord);

    normal = idShader.attributeLocation("qt_Normal");
    idShader.setAttributeBuffer(normal, GL_FLOAT,
                               6*4*3*sizeof(GLfloat)
                               +6*4*2*sizeof(GLfloat), 3, 0);
    idShader.enableAttributeArray(normal);


    idShader.setUniformValue("qt_Texture", 0);

    texture->bind(0);

//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    for(int i=0; i<6; i++)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
    }
//    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, (void*)0);
    texture->release();
    vBuffer.release();
    idShader.release();
}

void GraphicsCube
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

//    indexBuf.bind();

    GLuint position, texcoord, normal;
    position = depthShader.attributeLocation("qt_Vertex");
    depthShader.setAttributeBuffer(position, GL_FLOAT, 0, 3, 0);
    depthShader.enableAttributeArray(position);

    texcoord = depthShader.attributeLocation("qt_TexCoord");
    depthShader.setAttributeBuffer(texcoord, GL_FLOAT,
                               6*4*3*sizeof(GLfloat), 2, 0);
    depthShader.enableAttributeArray(texcoord);

    normal = depthShader.attributeLocation("qt_Normal");
    depthShader.setAttributeBuffer(normal, GL_FLOAT,
                               6*4*3*sizeof(GLfloat)
                               +6*4*2*sizeof(GLfloat), 3, 0);
    depthShader.enableAttributeArray(normal);


    depthShader.setUniformValue("qt_Texture", 0);

    texture->bind(0);

//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    for(int i=0; i<6; i++)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
    }
//    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, (void*)0);
    texture->release();
    vBuffer.release();
    depthShader.release();
}

void GraphicsCube::setLight(int index, LightProperty light)
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

    program.release();
}
