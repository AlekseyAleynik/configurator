#ifndef PORTBAUD_H
#define PORTBAUD_H

#include <QEventLoop>
#include <QTimer>
#include <QStringList>
#include <qdebug.h>

struct Pair
{
   QString port;
   QString baud;
};

class PortBaud
{
public:
    QVector<Pair> pairObjArr;
    PortBaud() {}
    PortBaud(Pair *_pairObjArr, int sizeArr)
    {
        for(int i = 0; i < sizeArr; i++)
            pairObjArr.append(_pairObjArr[i]);
    }
    PortBaud(QStringList _portList, QStringList _baudList)
    {
        for(int i = 0; i < _portList.size(); i++) {
            pairObjArr[i].port = _portList.at(i);
            pairObjArr[i].baud = _baudList.at(i);
        }
    }
    PortBaud(PortBaud &copyPb)
    {
        this->pairObjArr = copyPb.pairObjArr;
    }
    Pair getBaud(QString _port)
    {
        for(int i = 0; i < pairObjArr.size(); i++) {
            if(pairObjArr.at(i).port == _port)
                return pairObjArr.at(i);
        }
    }
    bool delPair(QString _port)
    {
        for(int i = 0; i < pairObjArr.size(); i++) {
            if(pairObjArr[i].port == _port)
            {
                pairObjArr.remove(i);
                return true;
            }
        }
        return false;
    }
    bool appendPair(QString _port, QString _baud)
    {
        Pair pr;
        pr.port = _port;
        pr.baud = _baud;
        for(int i = 0; i < pairObjArr.size(); i++) {
            if((pairObjArr.at(i).port == _port) && (pairObjArr.at(i).baud == _baud))
                return false;
        }
        pairObjArr.append(pr);
        return true;
    }
    void wait(int msec)
    {
        QEventLoop eventLoop;
        QTimer::singleShot(msec, &eventLoop, SLOT(quit()));
        //qApp->processEvents();
        eventLoop.exec();
    }
    ~PortBaud()
    {
    }
};

#endif // PORTBAUD_H
