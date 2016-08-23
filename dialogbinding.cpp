#include "dialogbinding.h"
#include "ui_dialogbinding.h"

DialogBinding::DialogBinding(int _bindPerInd, int _skipNum, QString controlPer, QVector<int> _arrChanVec[5],
                             QStringList _arrDevLst[5], QStringList slBindPer, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogBinding)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    arrDevLst = new QStringList[5];
    arrDevLst = _arrDevLst;
    arrChanVec = new QVector<int>[5];
    arrChanVec = _arrChanVec;
    ui->setupUi(this);
    slotChangeCls(1);
    slotChangeCountBinds(1);
    ui->cmbBxBindPeriod->addItems(slBindPer);
    ui->cmbBxBindPeriod->setCurrentIndex(_bindPerInd);
    ui->spnBxSkipNum->setValue(_skipNum);
    ui->lblControlPeriod->setText(controlPer);
    this->setWindowTitle("Связывание");
    ui->progressBarBinding->hide();
    connect(ui->spnBxCountBinds, SIGNAL(valueChanged(int)), this, SLOT(slotChangeCountBinds(int)));
    connect(ui->spnBxClass, SIGNAL(valueChanged(int)), this, SLOT(slotChangeCls(int)));
    connect(ui->pbCancelStart, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

DialogBinding::~DialogBinding()
{
    delete ui;
}

void DialogBinding::closeEvent(QCloseEvent *)
{
    emit signalBind(false);
    delete ui;
}

void DialogBinding::slotChangeCls(int _ind)
{
    ui->cmbBxTypeDev->clear();
    ui->cmbBxAddress->clear();
    ui->cmbBxTypeDev->addItem("Любой; 0x0000");
    ui->cmbBxTypeDev->addItems(arrDevLst[_ind]);
    ui->cmbBxTypeDev->setCurrentIndex(0);
    ui->spnBxClass->setValue(_ind);
    QStringList addrStrList;
    for(int i = 0; i < arrChanVec[_ind].size(); i++)
        addrStrList.append(QString::number(arrChanVec[_ind].at(i)));
    ui->cmbBxAddress->addItems(addrStrList);
    ui->cmbBxAddress->setCurrentIndex(0);
}

QString DialogBinding::getBindPer()
{
    return ui->cmbBxBindPeriod->currentText();
}

int DialogBinding::getSkipNum()
{
    return ui->spnBxSkipNum->value();
}

bool DialogBinding::parsStr()
{
    QString str = ui->cmbBxTypeDev->currentText();
    if(str.isEmpty())
        return false;
    else {
        int end = str.indexOf("0x");
        devName = str.left(end);
        str = str.right(4);
        //numCls = str[0].toAscii();
        ver = str[1].toAscii() - 48;
        bool ok;
        typeDev = str.right(2).toInt(&ok, 16);
        return ok;
    }
}

QString DialogBinding::getDevName()
{
    return devName;
}

int DialogBinding::getTypeDev()
{
    return typeDev;
}

int DialogBinding::getNumCls()
{
    return ui->spnBxClass->value();
}

int DialogBinding::getVer()
{
    return ver;
}

void DialogBinding::slotTimeout()
{
    int timeCounter = ui->progressBarBinding->value();
    if((timeCounter < 100) && (ui->pbCancelStart->text() == "Отмена"))
        ui->progressBarBinding->setValue(++timeCounter);
    else {
        ui->pbCancelStart->setText("Связать");
        emit signalBind(false);
        ui->progressBarBinding->setFormat("Связывание остановлено");
    }
}

void DialogBinding::slotClicked()
{
    if(ui->progressBarBinding->text() != "Связывание") {
        ui->progressBarBinding->show();
        ui->pbCancelStart->setText("Отмена");
        ui->progressBarBinding->setFormat("Связывание");
        emit signalBind(true);
    }
    else {
        countBinds = 0;
        ui->pbCancelStart->setText("Связать");
        ui->progressBarBinding->setFormat("Связывание остановлено");
        ui->progressBarBinding->setValue(0);
        emit signalBind(false);
    }
}

void DialogBinding::stopBindSlot()
{
    ui->progressBarBinding->setFormat("ОУ привязано");
    ui->pbCancelStart->setText("Связать");
    //ui->progressBarBinding->hide();
    ui->progressBarBinding->setValue(0);
}

void DialogBinding::slotChangeCountBinds(int _countBinds)
{
    countBinds = _countBinds;
}

int DialogBinding::getCountBinds()
{
    return countBinds;
}

int DialogBinding::getAdrress()
{
    /*if(ui->cmbBxAddress->currentIndex())
        return ui->cmbBxAddress->currentText().toInt();
    else
        return ui->cmbBxAddress->itemText(0).toInt();*/
    return ui->cmbBxAddress->currentText().toInt();
}

void DialogBinding::setArrFreeChanVec(QVector<int> vecFreeChans)
{
    ui->cmbBxAddress->clear();
    QStringList addrStrList;
    for(int i = 0; i < vecFreeChans.size(); i++)
        addrStrList.append(QString::number(vecFreeChans.at(i)));
    ui->cmbBxAddress->addItems(addrStrList);
    ui->cmbBxAddress->setCurrentIndex(0);
}
