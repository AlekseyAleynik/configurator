#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::itemModelDataChanged(int row, int col, QString newStr, QString oldStr)
{
    int numCls = clsNum();
    QModelIndex ind(proxyMdl->index(row, col));
    //QModelIndex ind = findSourceIndex(_ind);
    qDebug() << "ind.isValid() =" << ind.isValid() << "row | col =" << ind.row() << ind.column();
    if((oldStr == newStr) || (!ind.isValid()))
        return;
    QByteArray parBA = createParam(ind, newStr, numCls);
    NewComm *ncObj = new NewComm();
    proxyMdl->setData(ind, newStr);
    ui->pbSet->setEnabled(true);
    ui->pbCancel->setEnabled(true);
    if(ind.column() == 15)
        return;
    unsigned char commB = commByteCreate(ind.column());
    for(int i = 0; i < newCommList.size(); i++) {
        if((newCommList.at(i).cmnd.command == commB)
                && (newCommList.at(i).cmnd.numCh == ind.row())
                && (newCommList.at(i).cmnd.numClass == numCls)) {
            ncObj->oldVal = newCommList.at(i).oldVal;
            ncObj->ind = ind;
            //ncObj->newVal = newStr;
            for(int i = 0; i < parBA.size(); i++)
                ncObj->cmnd.param[i] = parBA[i];
            ncObj->cmnd.nParam = parBA.size();
            newCommList.removeAt(i);
            newCommList.insert(i, *ncObj);
            return;
        }
    }
    ncObj->ind = ind;
    ncObj->oldVal = oldStr;
    //ncObj->newVal = newStr;
    ncObj->cmnd.numClass = numCls;
    ncObj->cmnd.numCh = proxyMdl->data(proxyMdl->index(ind.row(), 31)).toInt();
    ncObj->cmnd.command = commB;
    for(int i = 0; i < parBA.size(); i++)
        ncObj->cmnd.param[i] = parBA[i];
    ncObj->cmnd.nParam = parBA.size();
    newCommList.push_front(*ncObj);
}

unsigned char MainWindow::commByteCreate(int numColumn)
{
    switch (numColumn) {
    case 6:
        return 0x19; // Кодирование
    case 7:
        return 0x07; // Индикация опознавания
    case 8:
        return 0x06; // Звуковая индикация состояния
    case 9:
        return 0x08; // Световая индикация состояния
    case 10:
        return 0x02; // Охрана
    case 11:
        return 0x12; // Пороги срабатывания Грань-РК
    case 12:
        return 0x12; // Верхний порог срабатывания аналоговых ОУ
    case 13:
        return 0x12; // Нижний порог срабатывания аналоговых ОУ
    case 14:
        return 0x29; // Режим работы исполнительных устройств
    case 16:
        return 0x12; // Длительность активной фазы режима
    case 17:
        return 0x12; // Длительность пассивной фазы режима
    default:
        return 0xff;
    }
}

QByteArray MainWindow::createParam(QModelIndex &ind, QString strNew, int numCls)
{
    QByteArray ba;
    ba[0] = proxyMdl->data(proxyMdl->index(ind.row(), 31)).toInt();
    if((ind.column() >= 6) && (ind.column() <= 10)) {
        if(strNew == "вкл.")
            ba[1] = (unsigned char)0x01;
        else
            ba[1] = (unsigned char)0x00;
    }
    if(ind.column() == 11) {
        /*0x12*/
        if((strNew == "1-min") || (strNew == "8-max"))
            strNew = strNew.left(1);
        else
            strNew = strNew.remove(QChar(176/*'°'*/));
        int par = strNew.toInt();
        if((proxyMdl->data(proxyMdl->index(ind.row(), 50)).toInt() > 0)
                && (proxyMdl->data(proxyMdl->index(ind.row(), 51)).toInt() == 1))
            par = translateChannelTwo(par - 1);
        ba[1] = par - 1;
    }
    if((ind.column() >= 14) && (ind.column() <= 17)) {
        int newValDig;
        if(strNew.toInt())
            newValDig = strNew.toInt();
        else {
            if(ind.column() == 14)
                newValDig = parceParamTrubach(strNew);
        }
        bool convertFlag;
        if((ind.column() >= 14) && (ind.column() <= 17)) {
            if(ind.column() == 14) {
                /*0x32*/
                //ba[1] = parceParamTrubach(mdl->mdlDt(ind.row(), 14).toString());
                ba[1] = newValDig;
                if(newValDig)
                    proxyMdl->setData(proxyMdl->index(ind.row(), 15), "");
                else
                    proxyMdl->setData(proxyMdl->index(ind.row(), 15), "Без ограничений");
                proxyMdl->setData(proxyMdl->index(ind.row(), 16), "wait");
                proxyMdl->setData(proxyMdl->index(ind.row(), 17), "wait");
                unsigned char ucPar[ba.size()];
                for(int i = 0; i < ba.size(); i++)
                    ucPar[i] = ba[i];
                Command cm(numCls,
                           ba[0],
                           0x32,
                           ucPar,
                           ba.size());
                commLst.push_front(cm);
            }
            if(ind.column() == 16 || ind.column() == 17) {
                /*0x33*/
                ba[1] = parceParamTrubach(proxyMdl->data(proxyMdl->index(ind.row(), 14)).toString());
                if(ind.column() == 16) {
                    ba[2] = newValDig;
                    ba[3] = proxyMdl->data(proxyMdl->index(ind.row(), 17)).toInt() / 100;
                }
                if(ind.column() == 17) {
                    ba[2] = proxyMdl->data(proxyMdl->index(ind.row(), 16)).toInt() / 100;
                    ba[3] = newValDig;
                }
                if(ba[2] == 0 || ba[3] == 0) {
                    ui->pbSet->setDisabled(true);
                    ui->pbCancel->setDisabled(true);
                }
            }
            if(ind.column() == 15) {
                /*0x29;*/
                if(proxyMdl->data(proxyMdl->index(ind.row(), 14)).toString().toInt(&convertFlag, 16))
                    numModeTrubachInt = proxyMdl->data(proxyMdl->index(ind.row(),
                                                                       14)).toString().toInt(&convertFlag, 16);
                else
                    numModeTrubachInt = parceParamTrubach(proxyMdl->data(proxyMdl->index(ind.row(),
                                                                                         14)).toString());
                ba[1] = numModeTrubachInt;
                ba[2] = parceParamTrubach_2(strNew);
            }
        }
    }
    if(ind.column() == 12 || ind.column() == 13) {

        int intPar1;
        int intPar2;
        if(ind.column() == 12) {
            intPar1 = proxyMdl->data(ind).toInt();
            intPar2 = proxyMdl->data(proxyMdl->index(ind.row(), 13)).toInt();
        }
        else {
            intPar1 = proxyMdl->data(proxyMdl->index(ind.row(), 12)).toInt();
            intPar2 = proxyMdl->data(ind).toInt();
        }
        if(intPar1 <= intPar2) {
            ui->pbSet->setDisabled(true);
            ui->pbCancel->setDisabled(true);
        }
        QByteArray pars1 = paramCreator(intPar1);
        QByteArray pars2 = paramCreator(intPar2);
        ba[1] = pars1[0];
        ba[2] = pars1[1];
        ba[3] = pars2[0];
        ba[4] = pars2[1];
    }
    return ba;
}

QByteArray MainWindow::paramCreator(int intPar1)
{
    bool ok = false;
    int p1b = QString::number(abs(intPar1), 10).toInt(&ok, 16);
    unsigned char par1b = p1b >> 0x08;
    if(intPar1 < 0)
        par1b = par1b | 0x80;
    int p2b = QString::number(abs(intPar1), 10).toInt(&ok, 16);
    unsigned char par2b = p2b & 0xff;
    QByteArray result;
    result[0] = par1b;
    result[1] = par2b;
    return result;
}

void MainWindow::slotSetSettings()
{
    for(int i = 0; i < newCommList.size(); i++)
        commLst.push_front(newCommList.takeFirst().cmnd);
    ui->pbSet->setDisabled(true);
    ui->pbCancel->setDisabled(true);
}

void MainWindow::slotCancelSettings()
{
    Command comm;
    NewComm *ncObj = new NewComm();
    while(newCommList.size()) {
        *ncObj = newCommList.takeFirst();
        proxyMdl->setData(ncObj->ind, ncObj->oldVal);
        comm.setCommand(ncObj->cmnd.numClass,
                        ncObj->cmnd.numCh,
                        0x01,
                        ncObj->cmnd.param,
                        1);
        commLst.push_front(comm);
    }
    ui->pbSet->setDisabled(true);
    ui->pbCancel->setDisabled(true);
}

int MainWindow::translateChannelTwo(int par)
{
    switch (par) {
    case 3:
        par = 1;
        break;
    case 10:
        par = 2;
        break;
    case 25:
        par = 3;
        break;
    case 45:
        par = 4;
        break;
    default:
        break;
    }
    return par;
}

int MainWindow::parceParamTrubach(QString str)
{
    if(str == "выключено")
        return 0;
    else if(str == "включено")
        return 1;
    else if(str == "режим 0.5 Гц")
        return 2;
    else if(str == "режим 1 Гц")
        return 3;
    else if(str == "режим 2 Гц")
        return 4;
    else if(str == "режим 4 Гц")
        return 5;
    else if(str.toInt())
        return str.toInt();
    else
        return -1;
}

int MainWindow::parceParamTrubach_2(QString str)
{
    if(str == "без ограничений")
        return 0;
    else if(str == "10 с")
        return 1;
    else if(str == "20 с")
        return 2;
    else if(str == "30 с")
        return 3;
    else if(str == "1 мин")
        return 6;
    else if(str == "5 мин")
        return 0x1e;
    else if(str == "10 мин")
        return 0x3c;
    else
        return -1;
}

void MainWindow::slotDelAll()
{
    QList<int> listFirstChansDev;
    int ch;
    unsigned char numChPar;
    for(int i = 0; i < mdl->rowCount(); i++) {
        ch = mdl->mdlDt(i, 31).toInt();
        if(!listFirstChansDev.contains(ch)) {
            listFirstChansDev.append(ch);
            numChPar = ch;
            Command comm(mdl->mdlDt(i, 35).toInt(),
                    ch,
                    0x0E,
                    &numChPar,
                    1);
            commLst.push_back(comm);
        }
    }
}

void MainWindow::slotOnForAll()
{
    QList<int> listFirstChansDev;
    int ch;
    unsigned char numChPar[2];
    numChPar[1] = 1;
    for(int i = 0; i < mdl->rowCount(); i++) {
        ch = mdl->mdlDt(i, 31).toInt();
        if(!listFirstChansDev.contains(ch) && (mdl->mdlDt(i, 2).toInt() > 0)) {
            listFirstChansDev.append(ch);
            numChPar[0] = ch;
            Command comm(mdl->mdlDt(i, 35).toInt(),
                    ch,
                    commByteCreate(tbl->currColumn),
                    numChPar,
                    2);
            commLst.push_back(comm);
        }
    }
}

void MainWindow::slotOffForAll()
{
    QList<int> listFirstChansDev;
    int ch;
    unsigned char numChPar[2];
    numChPar[1] = 0;
    for(int i = 0; i < mdl->rowCount(); i++) {
        ch = mdl->mdlDt(i, 31).toInt();
        if(!listFirstChansDev.contains(ch) && (mdl->mdlDt(i, 2).toInt() > 0)) {
            listFirstChansDev.append(ch);
            numChPar[0] = ch;
            Command comm(mdl->mdlDt(i, 35).toInt(),
                    ch,
                    commByteCreate(tbl->currColumn),
                    numChPar,
                    2);
            commLst.push_back(comm);
        }
    }
}

void MainWindow::slotDelSel()
{
    QList<int> listFirstChansDev;
    int ch;
    unsigned char numChPar;
    for(int i = 0; i < tbl->lstSelInd.size(); i++) {
        ch = proxyMdl->data(proxyMdl->index(tbl->lstSelInd.at(i).row(), 31)).toInt();
        if(!listFirstChansDev.contains(ch)) {
            listFirstChansDev.append(ch);
            numChPar = ch;
            Command comm(mdl->mdlDt(i, 35).toInt(),
                    ch,
                    0x0E,
                    &numChPar,
                    1);
            commLst.push_back(comm);
        }
    }
}

void MainWindow::slotOnForSel()
{
    QList<int> listFirstChansDev;
    int ch;
    unsigned char numChPar[2];
    numChPar[1] = 1;
    for(int i = 0; i < tbl->lstSelInd.size(); i++) {
        ch = proxyMdl->data(proxyMdl->index(tbl->lstSelInd.at(i).row(), 31)).toInt();
        if(!listFirstChansDev.contains(ch) && (mdl->mdlDt(i, 2).toInt() > 0)) {
            listFirstChansDev.append(ch);
            numChPar[0] = ch;
            Command comm(mdl->mdlDt(i, 35).toInt(),
                    ch,
                    commByteCreate(tbl->ind.column()),
                    numChPar,
                    2);
            commLst.push_back(comm);
        }
    }
}

void MainWindow::slotOffForSel()
{
    QList<int> listFirstChansDev;
    int ch;
    unsigned char numChPar[2];
    numChPar[1] = 0;
    for(int i = 0; i < tbl->lstSelInd.size(); i++) {
        ch = proxyMdl->data(proxyMdl->index(tbl->lstSelInd.at(i).row(), 31)).toInt();
        if(!listFirstChansDev.contains(ch) && (mdl->mdlDt(i, 2).toInt() > 0)) {
            listFirstChansDev.append(ch);
            numChPar[0] = ch;
            Command comm(mdl->mdlDt(i, 35).toInt(),
                    ch,
                    commByteCreate(tbl->ind.column()),
                    numChPar,
                    2);
            commLst.push_back(comm);
        }
    }
}
