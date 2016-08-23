#ifndef DIALOGSELECTCONNECTION_H
#define DIALOGSELECTCONNECTION_H

#include <QDialog>
#include <QTextCodec>
#include "serialdeviceenumerator.h"
#include "PortBaud.h"

namespace Ui {
class DialogSelectConnection;
}

class DialogSelectConnection : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectConnection(PortBaud *pbList, QWidget *parent = 0);
    void getFirstPort();
    ~DialogSelectConnection();
private:
    Ui::DialogSelectConnection *ui;
    PortBaud *pbLst;
private slots:
    void cmbbxPortChanged(QString _port);
signals:
    void tryConnSignal(QString, QString);
};

#endif // DIALOGSELECTCONNECTION_H
