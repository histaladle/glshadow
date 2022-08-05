#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
//    qDebug() << "view" << viewMat;
//    projection.rotate(M_PI/2, 0.0, 1.0, 0.0);
}

void OGLWidget::initializeGL()
{
    LightProperty light;
    light.isEnable=true;
    light.isLocal=true;
    light.isSpot=true;
    light.ambient=QVector3D(1.0,1.0,1.0);
    light.color=QVector3D(1.0,1.0,1.0);
    light.position=QVector3D(5.0,0.0,5.0);
    light.halfVector=(light.position+eye).normalized();
    light.focus=focus;
    light.spotCosCutoff=0.5;
    light.spotExponent=0.9;
    light.constantAttenuation=1.0;//1.0
    light.linearAttenuation=0.001;//0.09
    light.quadraticAttenuation=0.0;//0.032

    QMatrix4x4 projection;
    projection.perspective(45.0f, 1.0, 0.1f, 1000.0f);
    light.projection=projection;

    QMatrix4x4 view;
    view.lookAt(light.position,light.focus,QVector3D(0.0,1.0,0.0));
    light.view=view;

    light.texIndex=1;

    lights[0]=light;

    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
//    glDeleteFramebuffers(1,&fboId);
//    glDeleteTextures(1,&depthTexId);

//    glGenTextures(1, &depthTexId);
//    glBindTexture(GL_TEXTURE_2D, depthTexId);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 256, 256,
//                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
//                    GL_COMPARE_REF_TO_TEXTURE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    glGenFramebuffers(1, &fboId);
//    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
//                           GL_TEXTURE_2D, depthTexId, 0);
//    glDrawBuffer(GL_NONE);

    origin = new GraphicsOrigin;
    GraphicsCube *cube = new GraphicsCube;
//    items.append(cube);

    GraphicsTriangles *tris=new GraphicsTriangles;
    STLLoader stl;
    stl.parse("./boat.stl");
    stl.centerFacets();
    tris->setTriangles(stl.getFacets());
    items.append(tris);

    GraphicsBillBoard *board=new GraphicsBillBoard;
    board->setCenter(QVector3D(0.0,0.0,0.0));
//    boards.append(board);

    shd=new GraphicsShadow;
//    shd->setCenter(0.0,0.0,-1.0);
    shd->setCenter(0.0,0.0,tris->getMinZ()-0.001);
    origin->updateBuffer();
    for(int i=0; i<items.size(); i++)
    {
        GraphicsItem *item=nullptr;
        item = items[i];
        item->updateBuffer();
//        item->setLight(0,light);
    }
    for(int i=0;i<boards.size();i++)
    {
        GraphicsBillBoard *board=nullptr;
        board = boards[i];
        board->updateBuffer();
    }
}

void OGLWidget::paintGL()
{
    int w = width();
    int h = height();
//    int side = qMin(w, h);
//    glEnable(GL_DEPTH_TEST);
    renderDepthFromLight(0);
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glViewport(0.0, 0.0, w, h);
//    setViewPort(0.0, 0.0, w, h);
    glEnable(GL_DEPTH_TEST);
    QMatrix4x4 model, view, projection;
    projection.perspective(45.0f, (GLfloat)w/(GLfloat)h,
                           0.1f, 1000.0f);
//    projection.frustum(-1.0, 1.0, -w/h, w/h, 1.0, 1000.0);
//    projection.ortho(-w/200, w/200, -h/200, h/200, 0.1, 100.0);

    view.lookAt(eye,focus,up);

//    QMatrix4x4 tmat,rmat;
//    tmat.translate(displace.x(), displace.y(), displace.z());
//    rmat.rotate(xRot, 1.0, 0.0, 0.0);
//    rmat.rotate(yRot, 0.0, 1.0, 0.0);
//    rmat.rotate(zRot, 0.0, 0.0, 1.0);
//    model=tmat*rmat*tmat.inverted();

    model.translate(displace.x(), displace.y(), displace.z());
//    model.scale(scale);
    model.rotate(xRot, 1.0, 0.0, 0.0);
    model.rotate(yRot, 0.0, 1.0, 0.0);
    model.rotate(zRot, 0.0, 0.0, 1.0);

//    QMatrix4x4 bs=QMatrix4x4(0.5,0.0,0.0,0.5,
//                             0.0,0.5,0.0,0.5,
//                             0.0,0.0,0.5,0.5,
//                             0.0,0.0,0.0,1.0);
//    qDebug() << "debug10" << projection*view*model*QVector4D(5,5,0,1)
//             << displace;

    origin->draw(projection,view,model);
    for(int i=0; i<items.size(); i++)
    {
        GraphicsItem *item=nullptr;
        item = items[i];
        item->draw(projection,view,model);
    }

    for(int i=0;i<boards.size();i++)
    {
        GraphicsBillBoard *board=nullptr;
        board=boards[i];
        board->draw(projection,view,model);
    }
    shd->draw(projection,view,model);
}

void OGLWidget::renderDepthFromLight(int index)
{
    if(index>=MAX_LIGHT)
    {
        return;
    }
//    LightProperty light=lights[index];
    if(!lights[index].isEnable)
    {
        return;
    }
    int w = width();
    int h = height();
//    int side = qMin(w, h);
    QMatrix4x4 model, view, projection;
//    projection.perspective(qAcos(light.spotCosCutoff)*180/M_PI,
//                           (GLfloat)w/(GLfloat)h,
//                           0.1f, 1000.0f);
    projection.perspective(45.0f, 1.0,
                           0.1f, 1000.0f);
//    projection.frustum(-1.0, 1.0, -1.0, 1.0, 0.1, 1000.0);
//    projection.ortho(-w/200, w/200, -h/200, h/200, 0.1, 100.0);

//    view.lookAt(eye,focus,up);
    view.lookAt(lights[index].position,lights[index].focus,up);
    model.translate(displace.x(), displace.y(), displace.z());
//    model.scale(scale);
    model.rotate(xRot, 1.0, 0.0, 0.0);
    model.rotate(yRot, 0.0, 1.0, 0.0);
    model.rotate(zRot, 0.0, 0.0, 1.0);

//    if(!fbos[index])
//    {
//        QOpenGLFramebufferObjectFormat fbofmt;
//        fbofmt.setTextureTarget(GL_TEXTURE_2D);
//        fbofmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
//        fbos[index]=new QOpenGLFramebufferObject(1024,1024,fbofmt);
//    }
    if(!fbos[index] || fbos[index]->width()!=w || fbos[index]->height()!=h)
    {
        delete fbos[index];
        QOpenGLFramebufferObjectFormat fbofmt;
        fbofmt.setTextureTarget(GL_TEXTURE_2D);
        fbofmt.setAttachment(QOpenGLFramebufferObject::Depth);
        fbos[index]=new QOpenGLFramebufferObject(w,h,fbofmt);
    }
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    qDebug() << "fbo bind" << fbos[index]->bind() << w << h
             << fbos[index]->width() << fbos[index]->height()
             << fbos[index]->textures().size();

    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glViewport(0.0, 0.0, w, h);
//    setViewPort(0.0, 0.0, w, h);
//    glEnable(GL_DEPTH_TEST);
//    origin->renderDepth(projection,view,model);
    for(int i=0; i<items.size(); i++)
    {
        GraphicsItem *item=nullptr;
        item = items[i];
        item->renderDepth(projection,view,model);
    }
    for(int i=0;i<boards.size();i++)
    {
        GraphicsBillBoard *board=nullptr;
        board=boards[i];
        board->renderDepth(projection,view,model);
    }
    lights[index].texLoc=fbos[index]->texture();
    lights[index].texSize.setX(fbos[index]->width());
    lights[index].texSize.setY(fbos[index]->height());
    shd->setLight(0,lights[index]);
    for(int i=0; i<items.size(); i++)
    {
        GraphicsItem *item=nullptr;
        item = items[i];
        item->setLight(index,lights[index]);
    }
//    glDisable(GL_DEPTH_TEST);
    fbos[index]->release();
    QOpenGLFramebufferObject::bindDefault();
}

void OGLWidget::renderId()
{
    int w = width();
    int h = height();
//    int side = qMin(w, h);
    QMatrix4x4 model, view, projection;
    projection.perspective(45.0f,
                           (GLfloat)w/(GLfloat)h,
                           0.1f, 1000.0f);
//    projection.frustum(-w/200, w/200, -h/200, h/200, 1.0, 100.0);
//    projection.ortho(-w/200, w/200, -h/200, h/200, 0.1, 100.0);
    view.lookAt(eye,focus,up);
    model.translate(displace.x(), displace.y(), displace.z());
//    model.scale(scale);
    model.rotate(xRot, 1.0, 0.0, 0.0);
    model.rotate(yRot, 0.0, 1.0, 0.0);
    model.rotate(zRot, 0.0, 0.0, 1.0);
    if(!idfbo || idfbo->width()!=w || idfbo->height()!=h)
    {
        delete idfbo;
        QOpenGLFramebufferObjectFormat fbofmt;
        fbofmt.setTextureTarget(GL_TEXTURE_2D);
        fbofmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        idfbo=new QOpenGLFramebufferObject(w,h,fbofmt);
    }
    qDebug() << "fbo bind" << idfbo->bind() << w << h
             << idfbo->width() << idfbo->height();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glViewport(0.0, 0.0, w, h);
//    setViewPort(0.0, 0.0, w, h);
    glEnable(GL_DEPTH_TEST);

    for(int i=0; i<items.size(); i++)
    {
        GraphicsItem *item=nullptr;
        item = items[i];
        item->renderId(projection,view,model);
    }
    for(int i=0;i<boards.size();i++)
    {
        GraphicsBillBoard *board=nullptr;
        board=boards[i];
        board->renderId(projection,view,model);
    }
    glDisable(GL_DEPTH_TEST);
    idfbo->release();
}

void OGLWidget::saveFbo(QString filename)
{
    makeCurrent();
    if(filename.isEmpty())
    {
        return;
    }
    renderDepthFromLight(0);
    fbos[0]->toImage().save(filename);
//    img.save(filename);
}

void OGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float deltaX = 0.0;
    float deltaY = 0.0;
    deltaX = 1.0*(event->pos().x() - mousePosLast.x())/width();
    deltaY = 1.0*(event->pos().y() - mousePosLast.y())/height();
    if(event->buttons() == Qt::LeftButton)
    {
        deltaX *= 100;
        deltaY *= 100;

        if(event->modifiers() == Qt::ControlModifier)
        {
            zRot -= deltaY;
            while(zRot<0) zRot+=360;
        }
        else if(event->modifiers() == Qt::NoModifier)
        {
            xRot += deltaY;
            while(xRot>360) xRot-=360;

            yRot += deltaX;
            while(yRot>360) yRot-=360;
        }

        update();
    }
    else if(event->buttons() == Qt::RightButton)
    {
        deltaX *= -displace.z();
        deltaY *= -displace.z();
        displace+=QVector3D(deltaX, -deltaY, 0);
        update();
    }
    mousePosLast = event->pos();
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
    mousePosLast = event->pos();
//    renderId();
//    QImage img=idfbo->toImage();
//    if(event->button()==Qt::LeftButton)
//    {
//        if(rect().contains(event->pos()))
//        {
//            qDebug() << "select id"
//                     << event->pos()
//                     << QString::number(img.pixel(event->pos()),16);
//        }
//    }
}

void OGLWidget::wheelEvent(QWheelEvent *event)
{
    QMatrix4x4 t;
    int d = event->delta();
//    t.translate(0.0, 0.0, d/12.0);
//    glMatrixMode(GL_MODELVIEW);
//    glScalef(d/12.0, d/12.0, d/12.0);
    QVector3D p = scale;
    if(d < 0)
    {
        displace.setZ(displace.z()*1.1);
        scale.setX(p.x()*0.9);
        scale.setY(p.y()*0.9);
        scale.setZ(p.z()*0.9);
    }
    else if(d > 0)
    {
        displace.setZ(displace.z()*0.9);
        scale.setX(p.x()*1.1);
        scale.setY(p.y()*1.1);
        scale.setZ(p.z()*1.1);
    }
    update();
}

void OGLWidget::
setViewPort(int left, int right, int bottom, int top)
{
//    float zNear = 1.0;
//    float zFar = 100.0;
    float h = (float)(top - bottom);
    float w = (float)(right - left);
    float side = qMin(w, h);
    glViewport((w-side)/2, (h-side)/2, side, side);
//    glViewport(0.0, 0.0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(left,right,bottom,top,zNear,zFar);

//    float zNear = 1.0;
//    float zFar = 10.0;
//    float h = (float)(top - bottom);
//    float w = (float)(right - left);
//    glViewport(0.0, 0.0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glFrustum(left,right,bottom,top,zNear,zFar);
}

void OGLWidget::paintLineSegments()
{
    QList<QVector3D> route;
    for(int i = 0; i < spaceRoutes.size(); i ++)
    {
        route = spaceRoutes[i];
        paintLines(route);
    }
}

void OGLWidget::paintLines(QList<QVector3D> points)
{
    int pn = points.size();
    GLuint position, color;
    GLfloat v[3], c[3];
    QOpenGLBuffer vbo;
    vbo.create();
    vbo.bind();
    vbo.allocate(pn*3*2*sizeof(GLfloat));
    for(int i=0; i<pn-1; i++)
    {
        v[0]=points[i].x();
        v[1]=points[i].y();
        v[2]=points[i].z();

        c[0]=0.0;
        c[1]=0.0;
        c[2]=0.0;

        vbo.write(i*3*sizeof(GLfloat), v, 3*sizeof(GLfloat));
        position = program->attributeLocation("qt_Vertex");
        program->setAttributeBuffer(position, GL_FLOAT, 0, 3, 0);
        glEnableVertexAttribArray(position);

        vbo.write((pn+i)*3*sizeof(GLfloat), c, 3*sizeof(GLfloat));
        color = program->attributeLocation("qt_Color");
        program->setAttributeBuffer(color, GL_FLOAT,
                                    (pn+i)*3*sizeof(GLfloat), 3, 0);
        glEnableVertexAttribArray(color);

        glDrawArrays(GL_LINES, i, 2);
    }
    vbo.release();
}

void OGLWidget::paintOrigin()
{
    QMatrix4x4 transform;
    transform.setToIdentity();

    QVector3D vx(5.0, 0.0, 0.0);
    QVector3D vy(0.0, 5.0, 0.0);
    QVector3D vz(0.0, 0.0, 5.0);
    QVector3D c;
    QVector3D b1;
    QVector3D b2;
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
    QOpenGLBuffer vbo;
    vbo.create();
    vbo.bind();
    vbo.allocate(lvertices, 36*sizeof(GLfloat));

    GLuint position = program->attributeLocation("qt_Vertex");
    program->setAttributeBuffer(position, GL_FLOAT, 0, 3, 0);
    glEnableVertexAttribArray(position);

    vbo.write(18*sizeof(GLfloat), lcolors, 18*sizeof(GLfloat));

    GLuint color = program->attributeLocation("qt_Color");
    program->setAttributeBuffer(color, GL_FLOAT,
                                18*sizeof(GLfloat), 3, 0);
    glEnableVertexAttribArray(color);

    glDrawArrays(GL_LINES, 0, 6);
    vbo.release();
}

void OGLWidget::clearRoutes()
{
    for(int i = 0; i < spaceRoutes.size(); i ++)
    {
        spaceRoutes[i].clear();
    }
    spaceRoutes.clear();
}

bool OGLWidget::isBetween(QVector3D p1, QVector3D p2, QVector3D p3)
{
    if(p1 == p2 || p2 == p3)
    {
        return true;
    }
    bool matched = true;
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p2;
    float cos = QVector3D::dotProduct(v1, v2) /
            v1.length() / v2.length();
    matched &= (cos >= 1);
    return matched;
}
