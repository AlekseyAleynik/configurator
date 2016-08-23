#ifndef VECTOR2DINT_H
#define VECTOR2DINT_H

#include <QVector>

class Vector2dInt
{
public:
    Vector2dInt();
    Vector2dInt(int _sizeX, int _sizeY);
    int sizeX, sizeY;
    QVector< QVector< int > > vol;
private:
    int dftVal;
};

#endif // VECTOR2DINT_H
