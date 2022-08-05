#include "stlloader.h"

STLLoader::STLLoader()
{

}

QList<Facet>
STLLoader::parseFacetsFromSTL(QString fileName)
{
    QFile file(fileName);
    QList<Facet> facets;
    if(!file.exists())
    {
        return facets;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        return facets;
    }
    QTextStream stream(&file);
    QString all;
    all=stream.readAll().trimmed();
    file.close();
    if(all.split(" ").at(0)=="solid")
    {
        facets=parseFacetsFromAscii(fileName);
    }
    else //if(all.split(" ").at(0)=="Exported")
    {
        facets=parseFacetsFromBin(fileName);
    }
    return facets;
}

QList<Facet>
STLLoader::parseFacetsFromAscii(QString fileName)
{
    QFile file(fileName);
    QList<Facet> facets;
    if(!file.exists())
    {
        qDebug() << fileName << "not existed";
        return facets;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << fileName << "open failed";
        return facets;
    }
    QTextStream stream(&file);
    QString line;
    QStringList ss;
    Facet facet;
    int vindex;
    while(!stream.atEnd())
    {
        line=stream.readLine().trimmed();
        ss=line.split(" ");
        if(ss.at(0)=="solid")
        {
            if(ss.size()>=2)
            {
                head=line;
                qDebug() << "stl ascii head" << head;
            }
        }
        else if(ss.at(0)=="facet")
        {
            if(ss.size()>=5
                    &&ss.at(1)=="normal")
            {
                facet.normal.setX(ss.at(2).toFloat());
                facet.normal.setY(ss.at(3).toFloat());
                facet.normal.setZ(ss.at(4).toFloat());
            }
            else
            {
                break;
            }
        }
        else if(ss.at(0)=="outer")
        {
            if(ss.at(1)=="loop")
            {
                vindex=0;
                continue;
            }
            else
            {
                break;
            }
        }
        else if(ss.at(0)=="vertex")
        {
            if(ss.size()>=4)
            {
                if(vindex>=3)
                {
                    break;
                }
                facet.v[vindex].setX(ss.at(1).toFloat());
                facet.v[vindex].setY(ss.at(2).toFloat());
                facet.v[vindex].setZ(ss.at(3).toFloat());

                vindex++;
            }
            else
            {
                break;
            }
        }
        else if(ss.at(0)=="endloop")
        {
            continue;
        }
        else if(ss.at(0)=="endfacet")
        {
            facets.append(facet);
            continue;
        }
        else if(ss.at(0)=="endsolid")
        {
            continue;
        }
    }
    file.close();
    return facets;
}

QList<Facet>
STLLoader::parseFacetsFromBin(QString fileName)
{
    qDebug() << "stl bin";
    QFile file(fileName);
    QList<Facet> facets;
    if(!file.exists())
    {
        qDebug() << fileName << "not existed";
        return facets;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << fileName << "open failed";
        return facets;
    }
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    QString line;
    QStringList ss;
    Facet facet;

    file.seek(0);
    head.clear();
    head.append(file.readAll().mid(0,80));
    qDebug() << "stl bin head" << head;

    file.seek(0);


    int ln=0;
    int vn=0;
    file.seek(80);
    stream >> vn;
    float nx,ny,nz;
    float x[3],y[3],z[3];
    quint16 attr=0;
    qDebug() << "stl bin vn" << vn;
    while(ln < vn)
    {
        file.seek(84+ln*50+0+0);
        stream >> nx;
        file.seek(84+ln*50+0+4);
        stream >> ny;
        file.seek(84+ln*50+0+8);
        stream >> nz;

        file.seek(84+ln*50+12+0);
        stream >> x[0];
        file.seek(84+ln*50+12+4);
        stream >> y[0];
        file.seek(84+ln*50+12+8);
        stream >> z[0];

        file.seek(84+ln*50+24+0);
        stream >> x[1];
        file.seek(84+ln*50+24+4);
        stream >> y[1];
        file.seek(84+ln*50+24+8);
        stream >> z[1];

        file.seek(84+ln*50+36+0);
        stream >> x[2];
        file.seek(84+ln*50+36+4);
        stream >> y[2];
        file.seek(84+ln*50+36+8);
        stream >> z[2];

        file.seek(84+ln*50+48);
        stream >> attr;

        if(stream.status()==QDataStream::Ok)
        {
            facet.normal.setX(nx);
            facet.normal.setY(ny);
            facet.normal.setZ(nz);

            facet.v[0].setX(x[0]);
            facet.v[0].setY(y[0]);
            facet.v[0].setZ(z[0]);

            facet.v[1].setX(x[1]);
            facet.v[1].setY(y[1]);
            facet.v[1].setZ(z[1]);

            facet.v[2].setX(x[2]);
            facet.v[2].setY(y[2]);
            facet.v[2].setZ(z[2]);

            facets.append(facet);
        }
        else
        {
            qDebug() << "bin error" << ln;
            break;
        }
        ln++;
    }

    return facets;
}

void STLLoader::calTriangleStrips(QList<Facet> facets)
{
    QList<QList<OpenGLVertex>> strips;
    QList<OpenGLVertex> strip;
    QList<OpenGLVertex> appendedtri,delpoints;
    QList<int> indexes;
    QList<QList<int>> indexesList;
    OpenGLVertex strippoint;
    Facet facet;

    int stripLen=0;

    for(int i=0;i<facets.size();i++)
    {
        facet=facets[i];
        stripLen=strip.size();
        if(strip.size()<3)
        {
            strippoint.pos=facet.v[0];
            strippoint.normal=facet.normal;
            strip.append(strippoint);

            strippoint.pos=facet.v[1];
            strippoint.normal=facet.normal;
            strip.append(strippoint);

            strippoint.pos=facet.v[2];
            strippoint.normal=facet.normal;
            strip.append(strippoint);

            indexes.append(strip.size()-3);
            indexes.append(strip.size()-2);
            indexes.append(strip.size()-1);
        }
        else
        {
            delpoints.clear();

            for(int j=0;j<3;j++)
            {
                for(int k=0;k<3;k++)
                {
                    stripLen=strip.size();
                    if(strip[stripLen-3+k].pos==facet.v[j])
                    {
                        strippoint.pos=facet.v[j];
                        strippoint.normal=facet.normal;
                        delpoints.append(strippoint);

                        indexes.append(stripLen-3+k);
                    }
                }
            }
            if(delpoints.size()<2)
            {
                i --;
                strips.append(strip);
                indexesList.append(indexes);
                strip.clear();
                indexes.clear();
            }
            else
            {
                appendedtri.clear();

                strippoint.pos=facet.v[0];
                strippoint.normal=facet.normal;
                appendedtri.append(strippoint);

                strippoint.pos=facet.v[1];
                strippoint.normal=facet.normal;
                appendedtri.append(strippoint);

                strippoint.pos=facet.v[2];
                strippoint.normal=facet.normal;
                appendedtri.append(strippoint);

                for(int j=0;j<delpoints.size();j++)
                {
                    appendedtri.removeAll(delpoints[j]);
                }
                for(int j=0;j<appendedtri.size();j++)
                {
                    strip.append(appendedtri[j]);
                    indexes.append(strip.size()-1);
                }
            }
        }
    }
    if(strip.size()>0)
    {
        strips.append(strip);
        indexesList.append(indexes);
    }
    this->strips=strips;
    this->indexesList=indexesList;
}

void STLLoader::calTriangleStrips()
{
    calTriangleStrips(this->facets);
    qDebug() << "debug4" << this->strips.size()
             << this->indexesList.size();
}

void STLLoader::centerFacets()
{
    QVector3D center=QVector3D(0.0,0.0,0.0);
    int fn=facets.size();
    if(fn<=0)
    {
        return;
    }
    for(int i=0;i<fn;i++)
    {
        center+=facets[i].v[0];
        center+=facets[i].v[1];
        center+=facets[i].v[2];
    }
    center/=3*fn;
    qDebug()<<"debug5"<<center;
    for(int i=0;i<fn;i++)
    {
        facets[i].v[0]-=center;
        facets[i].v[1]-=center;
        facets[i].v[2]-=center;
    }
}

QList<Facet> STLLoader::getFacets()
{
    return facets;
}

void STLLoader::parse(QString fileName)
{
    this->facets=parseFacetsFromSTL(fileName);
}
