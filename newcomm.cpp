#include "newcomm.h"

NewComm::NewComm()
{}

NewComm::NewComm(QString _oldVal,/* QString _newVal,*/ Command _cmnd, QModelIndex _ind)
{
    oldVal = _oldVal;
    //newVal = _newVal;
    cmnd.setCommand(_cmnd);
    ind = _ind;
}
