#include "vector2dint.h"

Vector2dInt::Vector2dInt()
{
    dftVal = 0;
    sizeX = sizeY = 1;
    vol = QVector< QVector< int > >(sizeX, QVector< int >(sizeY, dftVal));
}

Vector2dInt::Vector2dInt(int _sizeX, int _sizeY)
{
    dftVal = 0;
    sizeX = _sizeX;
    sizeY = _sizeY;
    vol = QVector< QVector< int > >(sizeX, QVector< int >(sizeY, dftVal));
}
