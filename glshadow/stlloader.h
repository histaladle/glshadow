#ifndef STLLOADER_H
#define STLLOADER_H

#include <QObject>
#include <QVector3D>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDataStream>
#include "facet.h"
#include "openglvertex.h"
class STLLoader : public QObject
{
public:
    STLLoader();
public:
    void parse(QString fileName);
    QList<Facet> parseFacetsFromSTL(QString fileName);
    QList<Facet> parseFacetsFromAscii(QString fileName);
    QList<Facet> parseFacetsFromBin(QString fileName);

    QList<Facet> getFacets();

    void calTriangleStrips(QList<Facet> facets);
    void calTriangleStrips();

    void centerFacets();
private:
    QList<Facet> facets;
    QString head;
public:
    QList<QList<OpenGLVertex>> strips;
    QList<QList<int>> indexesList;
};

#endif // STLLOADER_H
