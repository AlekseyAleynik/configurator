#include "command.h"

Command::Command()
{
    numClass = 0;
    numCh = 0;
    command = 0;
    param[16];
    nParam = 0;
    counter = 0;
    time = 0;
}

Command::Command(int numClass_,
                 int numCh_,
                 unsigned char command_,
                 unsigned char param_[],
                 int nParam_,
                 int counter_,
                 int time_)
{
    setCommand(numClass_, numCh_, command_, param_, nParam_, counter_, time_);
}

Command::Command(const Command &obj)
{
    this->numClass = obj.numClass;
    this->numCh = obj.numCh;
    this->command = obj.command;
    this->nParam = obj.nParam;
    for(int i = 0; i < this->nParam; i++)
        this->param[i] = obj.param[i];
    this->counter = obj.counter;
    this->time = obj.time;
}

void Command::setCommand(const Command &obj)
{
    this->numClass = obj.numClass;
    this->numCh = obj.numCh;
    this->command = obj.command;
    this->nParam = obj.nParam;
    for(int i = 0; i < this->nParam; i++)
        this->param[i] = obj.param[i];
    this->counter = obj.counter;
    this->time = obj.time;
}

void Command::setCommand(int numClass_,
                         int numCh_,
                         unsigned char command_,
                         unsigned char param_[],
                         int nParam_,
                         int counter_,
                         int time_)
{
    command = command_;
    numClass = numClass_;
    numCh = numCh_;
    nParam = nParam_;
    for(int i = 0; i < nParam_; i++)
        param[i] = param_[i];
    counter = counter_;
    time = time_;
}

int Command::getDelay()
{
    QString delay0x("0x");
    for(int i = 1; i >= 0; i--)
        delay0x.append(QString::number(param[i], 16).toAscii());
    bool ok;
    return time = delay0x.toInt(&ok, 16);
}
