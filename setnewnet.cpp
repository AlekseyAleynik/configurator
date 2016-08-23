#include "setnewnet.h"
#include "ui_setnewnet.h"

SetNewNet::SetNewNet(int sysAddress, int netNumber, int liter, int skipNum, int bindPer, QStringList _slTC,
                     QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SetNewNet)
{
    ui->setupUi(this);
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    ui->spnBxSysAddress->setValue(sysAddress);
    ui->spnBxNN->setValue(netNumber);
    ui->spnBxLit->setValue(liter);
    ui->spnBxSkipNum->setValue(skipNum);
    ui->cmbBxBindPeriod->addItems(_slTC);
    ui->cmbBxBindPeriod->setCurrentIndex(bindPer);
    ui->lblControlPeriod->setText(calcPer(bindPer, skipNum));
    this->setWindowTitle("Установка параметров сети");
    connect(ui->spnBxSkipNum, SIGNAL(valueChanged(int)), this, SLOT(setPK(int)));
    connect(ui->cmbBxBindPeriod, SIGNAL(currentIndexChanged(int)), this, SLOT(setPK(int)));
}

SetNewNet::SetNewNet(int skipNum, int bindPer, QStringList _slTC, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SetNewNet)
{
    ui->setupUi(this);
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    ui->spnBxSysAddress->hide();
    ui->spnBxNN->hide();
    ui->spnBxLit->hide();
    ui->lblSysAddress->hide();
    ui->lblNN->hide();
    ui->lblLit->hide();
    ui->line->hide();
    ui->spnBxSkipNum->setValue(skipNum);
    ui->cmbBxBindPeriod->addItems(_slTC);
    ui->cmbBxBindPeriod->setCurrentIndex(bindPer);
    ui->lblControlPeriod->setText(calcPer(bindPer, skipNum));
    this->setWindowTitle("Установка параметров связи с ОУ");
    connect(ui->spnBxSkipNum, SIGNAL(valueChanged(int)), this, SLOT(setPK(int)));
    connect(ui->cmbBxBindPeriod, SIGNAL(currentIndexChanged(int)), this, SLOT(setPK(int)));
}

SetNewNet::~SetNewNet()
{
    delete ui;
}

QString SetNewNet::calcPer(int periodSec, int sni)
{
    periodControl = periodSec * (sni + 1);
    QTime t(0, 0, 0);
    QTime tpc;
    tpc = t.addSecs(periodControl);
    int h = tpc.hour();
    int m = tpc.minute();
    int s = tpc.second();
    QString strLblCP;
    if(h)
        strLblCP.append(QString::number(h) + " ч. ");
    if(m)
        strLblCP.append(QString::number(m) + " мин. ");
    if(s)
        strLblCP.append(QString::number(s) + " сек.");
    return strLblCP;
}

void SetNewNet::setPK(int)
{
    int bpi = ui->cmbBxBindPeriod->currentIndex();
    int sni = ui->spnBxSkipNum->value();
    ui->lblControlPeriod->setText(calcPer(bpi, sni));
}

int SetNewNet::getNetNum()
{
    return ui->spnBxNN->value();
}

int SetNewNet::getLiter()
{
    return ui->spnBxLit->value();
}

QString SetNewNet::getBindPeriod()
{
    return ui->cmbBxBindPeriod->currentText();
}

int SetNewNet::getSkip()
{
    return ui->spnBxSkipNum->value();
}

QString SetNewNet::getContrPer()
{
    return ui->lblControlPeriod->text();
}

int SetNewNet::getSysAddress()
{
    return ui->spnBxSysAddress->value();
}
