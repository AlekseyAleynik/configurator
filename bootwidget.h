#ifndef BOOTWIDGET_H
#define BOOTWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "mainwindow.h"
#include "boot_protocol.h"
#include "serialdeviceenumerator.h"
#include "QProgressDialog"
#include <QDebug>
#include <QTextCodec>

class MainWindow;

class BootWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BootWidget(MainWindow *main);
    ~BootWidget();
    boot_protocol *b_prot;
private:
    QByteArray data;
    MainWindow *pMainWindow;
    int ex;
    void error_occ(QString);
    QProgressDialog *pDlg;
signals:
    void signal_SendPacket(QByteArray);
    void signal_serial_data(QByteArray);
    void signal_refresh();
    void timerStart(int interval);
    void sign6f();
private slots:
    bool setupBoot();
    void setupText(QByteArray);
    void setupMessage(QString);   
    void setProgressBar(int);
    //void slot_data_ret();
public slots:
     QString open_dialog();
};

#endif // BOOTWIDGET_H
