#ifndef NEWCOMM_H
#define NEWCOMM_H

#include <QModelIndex>
#include "command.h"

class NewComm
{
public:
    NewComm();
    NewComm(QString _oldVal,/* QString _newVal,*/ Command _cmnd, QModelIndex _ind);
    Command cmnd;
    QString oldVal;
    QModelIndex ind;
    //QString newVal;
};

#endif // NEWCOMM_H
