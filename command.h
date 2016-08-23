#ifndef COMMAND_H
#define COMMAND_H
#include <QByteArray>
#include <QString>

namespace Ui
{
    class Command;
}

class Command
{
public:
    int numClass;
    int numCh;
    unsigned char command;
    unsigned char param[256];
    int nParam;
    int counter;
    int time;
    Command();
    Command(int numClass_,
            int numCh_,
            unsigned char command_,
            unsigned char param_[] = 0,
            int nParam_ = 0,
            int counter_ = 0,
            int time_ = 0);
    Command(const Command &obj);
    void setCommand(const Command &obj);
    void setCommand(int numClass_,
                    int numCh_,
                    unsigned char command_,
                    unsigned char param_[] = 0,
                    int nParam_ = 0,
                    int counter_ = 0,
                    int time_ = 0);
    int getDelay();
};

#endif // COMMAND_H
