#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadCommLst()
{
    ui->cmbBxSysCmnd->hide();
    parceFile("CMD_0_SYS.txt", 0);
    parceFile("CMD_1_SENS.txt", 1);
    parceFile("CMD_2_EXEC.txt", 2);
    parceFile("CMD_3_INTERF.txt", 3);
    parceFile("CMD_4_ANALOG.txt", 4);
    ui->cmbBxCls->setCurrentIndex(0);
    ui->cmbBxCmnd->addItems(commandsVec[0]);
}

void MainWindow::parceFile(QString fileName, int numCls)
{
    QFile *f = new QFile(fileName);
    QString s;
    QStringList lstPairs;
    if(f->exists()) {
        f->open(QIODevice::ReadOnly);
        QTextStream str(f);
        while(!str.atEnd()) {
            s = str.readLine();
            if(!s.contains("//"))
                lstPairs << s;
        }
    }
    commandsVec[numCls] = lstPairs;
}

void MainWindow::slotChangeCls(int currInd)
{
    ui->cmbBxCmnd->clear();
    ui->cmbBxCmnd->addItems(commandsVec[currInd]);
}

void MainWindow::slotChangeCmnd(QString cmnd)
{
    bool ok;
    int comm = cmnd.left(2).toInt(&ok, 16);
    if((ok == true) && (comm == 0x7f)) {
        for(int i = 0; i < commandsVec[0].size(); i++) {
            if((commandsVec[0].at(i).at(0) != 54) && (commandsVec[0].at(i).at(0) != 55))
                ui->cmbBxSysCmnd->addItem(commandsVec[0].at(i));
        }
        ui->cmbBxSysCmnd->show();
    }
    else {
        ui->cmbBxSysCmnd->hide();
        ui->cmbBxSysCmnd->clear();
    }
}

void MainWindow::slotPBSend()
{
    commLst.clear();
    if(!customCommFlag) {
        chckBxInterView->setChecked(false);
        customCommFlag = true;
    }
    mainCycle();
}

void MainWindow::sendCustomComm()
{
    bool ok;
    int size = 0;
    QStringList parStrLst = ui->leParams->text().split(' ', QString::SkipEmptyParts);
    size = parStrLst.size();
    int cls = ui->cmbBxCls->currentIndex();
    int comm = ui->cmbBxCmnd->currentText().left(2).toInt(&ok, 16);
    int ch = -1;
    if(comm == 0x7f)
        cls = 0;
    if((comm != 0x7f) && (cls == 0))
        cls = -1;
    unsigned char params[size];
    for(int i = 0; i < size; i++)
        params[i] = parStrLst.at(i).toInt(&ok, 16);
    if(((comm == 0x7f) || (cls == 0)) && (size > 0)) {
        comm = ui->cmbBxSysCmnd->currentText().left(2).toInt(&ok, 16);
        ch = parStrLst.first().toInt(&ok, 16);
    } else {
        if(cls != -1) {
            ch = params[0];
            comm -= (cls - 1) * 0x20;
            if(cls == 4)
                comm -= 0x20;
        }
    }
    int repeateByte = 0;
    if(ui->chkBxRepeat->isChecked())
        repeateByte = 1;
    Command commObj(cls, ch, comm, params, size, repeateByte, 0);
    commLst.push_front(commObj);
    sendPack(commObj);
    while(ui->chkBxTimer->isChecked())
        sendPack(commObj);
}
