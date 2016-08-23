#include "dialogselectconnection.h"
#include "ui_dialogselectconnection.h"

DialogSelectConnection::DialogSelectConnection(PortBaud *_pbLst, QWidget *parent)
        :QDialog(parent), ui(new Ui::DialogSelectConnection)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    ui->setupUi(this);
    ui->lblConn->hide();
    this->setWindowTitle("Выбор соединения");
    pbLst = _pbLst;
    for(int i = 0; i < _pbLst->pairObjArr.size(); i++)
        ui->cmbbxPort->addItem(_pbLst->pairObjArr[i].port + " - " + _pbLst->pairObjArr[i].baud);
    if(pbLst->pairObjArr.size())
        ui->cmbbxPort->setCurrentIndex(0);
    connect(ui->cmbbxPort, SIGNAL(currentIndexChanged(QString)), this, SLOT(cmbbxPortChanged(QString)));
}

DialogSelectConnection::~DialogSelectConnection()
{
    delete ui;
}

void DialogSelectConnection::cmbbxPortChanged(QString _port)
{
    emit tryConnSignal(_port, pbLst->getBaud(_port).baud);
}

void DialogSelectConnection::getFirstPort()
{
    emit tryConnSignal(pbLst->pairObjArr[0].port, pbLst->pairObjArr[0].baud);
}
