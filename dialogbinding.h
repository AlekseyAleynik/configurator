#ifndef DIALOGBINDING_H
#define DIALOGBINDING_H

#include <QDialog>
#include <QTextCodec>
#include <QTimer>
#include <QDebug>
#include "command.h"

extern int periodControl;

namespace Ui {
class DialogBinding;
}

class DialogBinding : public QDialog
{
    Q_OBJECT
public:
    explicit DialogBinding(int _bindPerInd, int _skipNum, QString controlPer, QVector<int> _arrChanVec[5],
                           QStringList _arrDevLst[5],  QStringList slBindPer, QWidget *parent = 0);
    ~DialogBinding();
    bool parsStr();
    QString getDevName();
    int getTypeDev();
    int getNumCls();
    int getVer();
    QString getBindPer();
    int getSkipNum();
    int getCountBinds();
    int getAdrress();
    void stopBindSlot();
    void setArrFreeChanVec(QVector<int> vecFreeChans);
private:
    QString devName;
    int typeDev;
    unsigned char ver;
    Ui::DialogBinding *ui;
    QStringList *arrDevLst;
    QVector<int> *arrChanVec;
    int countBinds;
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void slotChangeCls(int _ind);
    void slotChangeCountBinds(int _countBinds);
public slots:
    void slotTimeout();
    void slotClicked();
signals:
    void signalBind(bool);
};

#endif // DIALOGBINDING_H
