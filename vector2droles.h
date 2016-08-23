#ifndef VECTOR2DROLES_H
#define VECTOR2DROLES_H

#include <QVector>
#include <QColor>

class Vector2dRoles
{
public:
    Vector2dRoles();
    Vector2dRoles(int _sizeX, int _sizeY);
    void appendRow(QColor _clr, QString _str);
    void delRow(int numRow);
    QVector< QVector< QString > > str;
    QVector< QVector< QColor > > clr;
private:
    int sizeY;
    QString dftStr;
    QColor dftClr;
};

#endif // VECTOR2DROLES_H
