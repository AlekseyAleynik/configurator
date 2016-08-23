#include "vector2droles.h"
#include <QDebug>

Vector2dRoles::Vector2dRoles(int _sizeX, int _sizeY)
{
    dftStr = "";
    dftClr = QColor(Qt::gray);
    sizeY = _sizeY;
    str = QVector< QVector< QString > >(_sizeX, QVector< QString >(_sizeY, dftStr));
    clr = QVector< QVector< QColor > >(_sizeX, QVector< QColor >(_sizeY, dftClr));
}

void Vector2dRoles::appendRow(QColor _clr, QString _str)
{
    QVector<QString> strVec = QVector<QString>(sizeY, _str);
    QVector<QColor> clrVec = QVector<QColor>(sizeY, _clr);
    str.append(strVec);
    clr.append(clrVec);
}

void Vector2dRoles::delRow(int numRow)
{
    str.remove(numRow);
    clr.remove(numRow);
}
