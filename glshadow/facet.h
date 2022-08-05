#ifndef FACET_H
#define FACET_H

#include <QVector3D>
struct Facet
{
    QVector3D v[3];
    QVector3D normal;
    quint16 attribute;
};

#endif // FACET_H
