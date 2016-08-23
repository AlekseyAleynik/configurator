#ifndef SETNEWNET_H
#define SETNEWNET_H

#include <QDialog>
#include <QTextCodec>
#include <QTime>
#include <QDebug>

extern int periodControl;

namespace Ui {
class SetNewNet;
}

class SetNewNet : public QDialog
{
    Q_OBJECT
public:
    explicit SetNewNet(int sysAddress, int netNumber, int liter, int skipNum,
                       int bindPer, QStringList slTC, QWidget *parent = 0);
    explicit SetNewNet(int skipNum, int bindPer, QStringList slTC, QWidget *parent = 0);
    ~SetNewNet();
    static QString calcPer(int periodSec, int sni);
    int getNetNum();
    int getLiter();
    QString getBindPeriod();
    int getSkip();
    int getSysAddress();
    QString getContrPer();
private:
    Ui::SetNewNet *ui;
private slots:
    void setPK(int);
};

#endif // SETNEWNET_H
