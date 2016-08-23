#include "mainwindow.h"
#include "ui_mainwindow.h"

extern int s_cnt_sys_bytes;
extern int s_cnt_start_bytes;
extern int s_num_rep_byte;
extern int s_num_s_com_byte;
extern int s_num_c_com_byte;
extern int numBytes0x15com;
extern unsigned char global_dev_type;
extern unsigned char broadcast_adr;
extern Protocol485RK *pprotocol485RK;
extern ProtocolSIGMArk *pprotocolSigma;
extern Protocol *pprotocol;

#define TimePoints 10000

#define SignalErr7911 "Не подключен датчик"
#define SignalErr7912 "Не подключено питание датчика"
#define SignalErr7913 "КЗ между каналом и землей"
#define SignalErr7914 "КЗ между двумя каналами"
#define SignalErr7915 "Ошибка времени преобразования"
#define SignalErr7916 "Отключение датчика после преобразования"
#define SignalErr7917 "Ошибка CRC"
#define SignalErr7918 "Ошибка конфигурации датчика"
#define SignalErr7919 "Считанная температура не находиться в диапазоне от -55 до +125"

void MainWindow::parce0x15()
{
    Command comm;
    unsigned char parAddress;
    matrixObj0x15 = new Vector2dInt(5, 128);
    for(int i = 0; i < numBytes0x15com; i++) {
        for(int j = 0; j < 8; j++) {
            if(currAnsw.param[i] & (int)pow(2, j)) {
                parAddress = i * 8 + j + 1;
                matrixObj0x15->vol[currAnsw.numClass][parAddress - 1] = 1;
                comm.setCommand(currAnsw.numClass,
                                   parAddress,
                                   0x14,
                                   &parAddress,
                                   1);
                commLst.push_front(comm);
                comm.setCommand(currAnsw.numClass,
                                   parAddress,
                                   0x01,
                                   &parAddress,
                                   1);
                commLst.push_front(comm);
                comm.setCommand(currAnsw.numClass,
                                   parAddress,
                                   0x0C,
                                   &parAddress,
                                   1);
                commLst.push_front(comm);
            }
        }
    }
}

void MainWindow::slotSortTblView(int col, Qt::SortOrder so)
{
    if(currComm.command != 0x14)
        tbl->horizontalHeader()->setSortIndicatorShown(true);
    else
        tbl->horizontalHeader()->setSortIndicatorShown(false);
    tbl->clearSpans();
    proxyMdl->sort(col, so);
    int counterCh, numChannels;
    for(int r = 0; r < proxyMdl->rowCount(); r++) {
        numChannels = proxyMdl->data(proxyMdl->index(r, 51)).toInt();
        counterCh = 0;
        if((numChannels > 1) && (numChannels == proxyMdl->data(proxyMdl->index(r, 52)).toInt())) {
            for(int j = 0; j < numChannels; j++) {
                if((r + j >= 0) && (r + j <= proxyMdl->rowCount())
                        && (proxyMdl->data(proxyMdl->index(r + j, 0)).toInt()
                        == proxyMdl->data(proxyMdl->index(r, 0)).toInt())) {
                    counterCh++;
                }
                else
                    break;
            }
            if(counterCh > 1) {
                for(int k = 0; k < 11; k++)
                    tbl->setSpan(r, k, counterCh, 1);
            }
            if(counterCh > 1) {
                for(int k = 18; k <= numVisibleModelColumns; k++)
                    tbl->setSpan(r, k, counterCh, 1);
            }
        }
        //tbl->setRowHeight(r, 30 * numChannels + 1);
        tbl->setRowHeight(r, 80);
    }
}

void MainWindow::parce0x14()
{
    int r;
    if(mdl->rowMdl(currAnsw.numCh, currAnsw.numClass) == -1) {
        r = mdl->rowCount();
        mdl->insertRows(r, 1);
    }
    else
        r = mdl->rowMdl(currAnsw.numCh, currAnsw.numClass);
    for(int c = 0; c < mdl->columnCount(); c++)
        mdl->setMdlDt(r, c, QColor(Qt::white), Qt::BackgroundColorRole);
    int OUSerial;
    QString mainStr;
    int byteNum = 0;
    unsigned char netAddr = currAnsw.param[byteNum++] & 31;
    mdl->setMdlDt(r, 0, netAddr);
    OUSerial = currAnsw.param[byteNum++] + currAnsw.param[byteNum++];
    mdl->setMdlDt(r, 26, QString::number(OUSerial, 16));
    int nChan = (currAnsw.nParam - 3) / 5 + 1;
    if(nChan > 1) {
        mainStr.append("Число каналов: " + QString::number(nChan) + '\n');
        mainStr = separateStr("Адрес 1-го канала: " + QString::number(currAnsw.param[byteNum]) + '\n', mainStr);
    }
    else
        mainStr = separateStr("Адрес канала: " + QString::number(currAnsw.param[byteNum]) + '\n', mainStr);
    for(int i = 0; i < nChan; i++) {
        mdl->setMdlDt(r, 31 + i, currAnsw.param[byteNum]);
        mdl->setMdlDt(r, 53, currAnsw.numCh);
        tbl->showRow(r);
        int keySpec = currAnsw.param[++byteNum] << 8;
        mdl->setMdlDt(r, 35 + i, currAnsw.param[byteNum] >> 4);
        mainStr = separateStr("Класс: " + QString::number(currAnsw.param[byteNum] >> 4) + '\n', mainStr);
        mdl->setMdlDt(r, 39 + i, QString::number(currAnsw.param[byteNum] & 15, 16));
        mainStr = separateStr("Версия: " + QString::number(currAnsw.param[byteNum++] & 15, 16) + '\n', mainStr);
        keySpec += currAnsw.param[byteNum];
        mdl->setMdlDt(r, 43 + i, QString::number(currAnsw.param[byteNum], 16));
        mainStr = separateStr("Тип: " + QString::number(currAnsw.param[byteNum++], 16) + '\n', mainStr);
        mdl->setMdlDt(r, 47 + i, QString::number(currAnsw.param[byteNum], 16));
        mainStr = separateStr("Хар.-ка: " + QString::number(currAnsw.param[byteNum++], 16) + '\n', mainStr);
        QString devName = devSpecificationMap.value(keySpec);
        if(devName.isEmpty())
            mdl->setMdlDt(r, 27, "Неизвестное ОУ");
        else
            mdl->setMdlDt(r, 27, devSpecificationMap.value(keySpec));
        int counterCh = 0;
        for(int i = 0; i < mdl->rowCount(); i++) {
            if(mdl->mdlDt(i, 0).toInt() == netAddr)
                counterCh++;
        }
        mdl->setMdlDt(r, 52, counterCh);
        /*counterCh = 1;
        if(mdl->mdlDt(r, 52).toInt() == nChan) {
            for(int j = 0; j < nChan; j++) {
                if(mdl->mdlDt(r + j - nChan + 1, 0).toInt() == netAddr)
                    counterCh++;
            }
            for(int k = 0; k < numVisibleModelColumns - 2; k++)
                tbl->setSpan(r - nChan + 1, k, counterCh, 1);
            qDebug() << "r - nChan + 1" << r - nChan;
        }*/
    }
    mainStr.insert(0, mdl->mdlDt(r, 27).toString() + "\n");
    tbl->setRowHeight(r, 80);
    mdl->setMdlDt(r, 1, mainStr);
    /*Command comm;
    unsigned char param;
    if((mdl->mdlDt(r, 35).toInt() == 4) || ((mdl->mdlDt(r, 35).toInt() == 1)
            && (mdl->mdlDt(r, 39).toInt() == 8) && (mdl->mdlDt(r, 47).toInt() == 8))) {
        param = mdl->mdlDt(r, 31).toInt();
        comm.setCommand(mdl->mdlDt(r, 35).toInt(), param, 0x0A, &param, 1, 0, 0);
        commLst.push_front(comm);
    }*/
    QVector<int> numUnic;
    for(int i = 0; i < 4; i++) {
        int currAddr = mdl->mdlDt(r, 31 + i).toInt();
        if((!numUnic.contains(currAddr)) && (currAddr != 0))
            numUnic.append(currAddr);
    }
    mdl->setMdlDt(r, 51, QString::number(numUnic.size()));
    for(int i = 0; i < nChan; i++)
        slotSortTblView();
}

void MainWindow::addSpecCommands(int row)
{
    int cls = mdl->mdlDt(row, 35).toInt();
    int ver = mdl->mdlDt(row, 39).toInt();
    int type = mdl->mdlDt(row, 43).toInt();
    int feature = mdl->mdlDt(row, 47).toInt();
    if((mdl->mdlDt(row, 2, Qt::BackgroundColorRole) == QColor(Qt::white)) && (mdl->mdlDt(row, 2).toInt() != 0)
            && (((cls == 1) && (ver == 4) && (type == 31) && (feature == 20)) || (cls > 1))) {
        int numCh = mdl->mdlDt(row, 51).toInt();
        Command comm;
        if(cls == 2) {
            unsigned char param[2];
            for(int i = 0; i < numCh; i++) {
                param[0] = mdl->mdlDt(row, 31 + i).toInt();
                param[1] = mdl->mdlDt(row, 14).toInt();
                comm.setCommand(cls, param[0], 0x12, param, 1, 0, 0);
                commLst.push_front(comm);
            }
        }
        else {
            unsigned char param;
            for(int i = 0; i < numCh; i++) {
                param = mdl->mdlDt(row, 31 + i).toInt();
                comm.setCommand(cls, param, 0x12, &param, 1, 0, 0);
                commLst.push_front(comm);
            }
        }
    }
}

QString MainWindow::separateStr(QString shortStr, QString mainStr)
{
    if(!mainStr.contains(shortStr))
        mainStr.append(shortStr);
    return mainStr;
}

void MainWindow::parce0x01()
{
    int r = mdl->rowMdl(currAnsw.numCh, currAnsw.numClass);
    if(!arrOccupiedChans[currAnsw.numClass].contains(currAnsw.numCh))
        arrOccupiedChans[currAnsw.numClass].append(currAnsw.numCh);
    int lvl = currAnsw.param[3];
    if(lvl > 0) {
        mdl->setMdlDt(r, 2, QColor(Qt::white), Qt::BackgroundColorRole);
        mdl->setMdlDt(r, 2, lvl);
        int netAddr = mdl->mdlDt(r, 0).toInt();
        QString str = QDate::currentDate().toString("dd.MM.yy") +
                " | " + QTime::currentTime().toString("hh:mm:ss");
        if(graph[netAddr].size() > TimePoints) {
            graph[netAddr].remove(graph[netAddr].begin().key());
            graph[netAddr].insert(str, lvl);
        }
        else
            graph[netAddr].insert(str, lvl);
    }
    else
        mdl->setMdlDt(r, 2, QColor(Qt::red), Qt::BackgroundColorRole);
    addSpecCommands(r);
    parceParam0x01(currAnsw);
    Command comm;
    unsigned char param;
    if((mdl->mdlDt(r, 2, Qt::BackgroundColorRole) == QColor(Qt::white)) && (mdl->mdlDt(r, 2).toInt() != 0)
            && ((mdl->mdlDt(r, 35).toInt() == 4) || ((mdl->mdlDt(r, 35).toInt() == 1)
                && (mdl->mdlDt(r, 39).toInt() == 8) && (mdl->mdlDt(r, 47).toInt() == 8)))) {
        param = mdl->mdlDt(r, 31).toInt();
        comm.setCommand(mdl->mdlDt(r, 35).toInt(), param, 0x0A, &param, 1, 0, 0);
        commLst.push_front(comm);
    }
}

void MainWindow::parce0x0f()
{
    int lvl = currAnsw.param[0];
    mdl->setMdlDt(mdl->rowMdl(currAnsw.numCh, currAnsw.numClass), 2, lvl);
    int netAddr = mdl->mdlDt(mdl->rowMdl(currAnsw.numCh, currAnsw.numClass), 0).toInt();
    QString str = QDate::currentDate().toString("dd.MM.yy") + " | " + QTime::currentTime().toString("hh:mm:ss");
    if(lvl > 0) {
        if(graph[netAddr].size() > TimePoints) {
            graph[netAddr].remove(graph[netAddr].begin().key());
            graph[netAddr].insert(str, lvl);
        }
        else
            graph[netAddr].insert(str, lvl);
    }
    parceParam0x01(currAnsw);
}

void MainWindow::parceParam0x01(Command &_comm)
{
    switch (_comm.numClass) {
    case 0:
        cellWork(_comm, 0, -1, 4);
        cellWork(_comm, 1, 3, 20, "Авария", "Подключение");
        if((_comm.numCh > 0) && ((mdl->mdlDt(mdl->rowMdl(_comm.numCh, _comm.numClass), 47).toInt() & 2) == 1))
            cellWork(_comm, 1, 4, 21, "Неисправность\nРП", "Норма РП");
        cellWork(_comm, 1, 5, 22, "Неисправность\nОП", "Норма ОП");
        cellWork(_comm, 1, 6, 23, "Вскрыт", "Закрыт");
        cellWork(_comm, 2, 4, 6, "Вкл", "Выкл");
        cellWork(_comm, 2, 6, 7, "Вкл", "Выкл");
        cellWork(_comm, 2, 7, 9, "Вкл", "Выкл");
        //cellWork(_comm, 2, 7, 9, "Световая\nиндикация\nсостояния\nразрешена",
          //       "Световая\nиндикация\nсостояния\nзапрещена");
        return;
    case 1:
        cellWork(_comm, 0, -1, 4);
        cellWork(_comm, 1, 2, 24, "Запыление", "Запыление устранено");
        cellWork(_comm, 1, 3, 20, "Авария", "Подключение");
        if((_comm.numCh > 0) && ((mdl->mdlDt(mdl->rowMdl(_comm.numCh, _comm.numClass), 47).toInt() & 2) == 1))
            cellWork(_comm, 1, 4, 21, "Неисправность\nРП", "Норма РП");
        cellWork(_comm, 1, 5, 22, "Неисправность\nОП", "Норма ОП");
        cellWork(_comm, 1, 6, 23, "Вскрыт", "Закрыт");
        cellWork(_comm, 2, 0, 10, "Вкл", "Выкл");
        cellWork(_comm, 2, 3, 6, "Вкл", "Выкл");
        cellWork(_comm, 2, 6, 7, "Вкл", "Выкл");
        cellWork(_comm, 2, 7, 9, "Вкл", "Выкл");
        cellWork(_comm, 2, 5, 8, "Вкл", "Выкл");
        //cellWork(_comm, 2, 7, 9, "Световая\nиндикация\nсостояния\nразрешена",
          //       "Световая\nиндикация\nсостояния\nзапрещена");
        //cellWork(_comm, 2, 5, 8, "Звуковая\nиндикация\nсостояния\nразрешена",
          //       "Звуковая\nиндикация\nсостояния\nзапрещена");
        return;
    case 2:
        cellWork(_comm, 0, -1, 4);
        cellWork(_comm, 1, 4, 25, "Неисправность\nдополнительного\nпитания", "Норма\nдополнительного\nпитания");
        cellWork(_comm, 1, 3, 20, "Авария", "Подключение");
        if((_comm.numCh > 0) && ((mdl->mdlDt(mdl->rowMdl(_comm.numCh, _comm.numClass), 47).toInt() & 2) == 1))
            cellWork(_comm, 1, 4, 21, "Неисправность\nРП", "Норма\nРП");
        cellWork(_comm, 1, 5, 22, "Неисправность\nОП", "Норма\nОП");
        cellWork(_comm, 1, 6, 23, "Вскрыт", "Закрыт");
        cellWork(_comm, 2, 0, 10, "Вкл", "Выкл");
        cellWork(_comm, 2, 3, 6, "Вкл", "Выкл");
        cellWork(_comm, 2, 6, 7, "Вкл", "Выкл");
        return;
    case 3:
        cellWork(_comm, 0, -1, 4);
        cellWork(_comm, 1, 5, 21, "Неисправность ОП", "Норма ОП");
        cellWork(_comm, 1, 6, 23, "Вскрыт", "Закрыт");
        cellWork(_comm, 2, 3, 6, "Вкл", "Выкл");
        if(_comm.param[0] & 128) {
            unsigned char parAddress = _comm.numCh;
            Command comm(_comm.numClass,
                         parAddress,
                         0x01,
                         &parAddress,
                         1);
            commLst.append(comm);
        }
        return;
    case 4:
        cellWork(_comm, 0, -1, 4);
        cellWork(_comm, 1, 3, 20, "Авария", "Подключение");
        if((_comm.numCh > 0) && ((mdl->mdlDt(mdl->rowMdl(_comm.numCh, _comm.numClass), 47).toInt() & 2) == 1))
            cellWork(_comm, 1, 4, 21, "Неисправность\nРП", "Норма РП");
        cellWork(_comm, 1, 5, 22, "Неисправность\nОП", "Норма ОП");
        cellWork(_comm, 1, 6, 23, "Вскрыт", "Закрыт");
        cellWork(_comm, 2, 0, 10, "Вкл", "Выкл");
        cellWork(_comm, 2, 3, 6, "Вкл", "Выкл");
        cellWork(_comm, 2, 6, 7, "Вкл", "Выкл");
        cellWork(_comm, 2, 7, 9, "Вкл", "Выкл");
        //cellWork(_comm, 2, 5, 8, "Звуковая\nиндикация\nсостояния\nразрешена",
          //       "Звуковая\nиндикация\nсостояния\nзапрещена");
        return;
    }
}

void MainWindow::cellWork(Command &_comm, int numByte, int bitVal, int col, QString mess1, QString mess2)
{
    int cl = _comm.numClass;
    int ch = _comm.numCh;
    int r = mdl->rowMdl(ch, cl);
    if(r == -1)
        return;
    QModelIndex ind = mdl->index(r, col);
    qDebug() << "numByte =" << numByte << " col =" << col << "cl =" << cl << " r =" << r << " ch =" << ch;
    switch (numByte) {
    case 0:
        switch (cl) {
        case 0:
            break;
        case 1:
            switch (_comm.param[numByte] & 3) {
            case 0:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::red)) {
                    mdl->setMdlDt(ind, QColor(Qt::white), Qt::BackgroundColorRole);
                    if(!mdl->mdlDt(ind).toString().isEmpty() && mdl->mdlDt(ind).toString() != "Восстановление") {
                        mdl->setMdlDt(ind, "Восстановление");
                        writeLog("Восстановление", ch, cl, true);
                    }
                    else
                        mdl->setMdlDt(ind, "");
                }
                else
                    mdl->setMdlDt(ind, "");
                break;
            case 1:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Нарушение") {
                        mdl->setMdlDt(ind, "Нарушение");
                        writeLog("Нарушение", ch, cl, true);
                    }
                }
                break;
            case 2:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "КЗ") {
                        mdl->setMdlDt(ind, "КЗ");
                        writeLog("КЗ", ch, cl, true);
                    }
                }
                break;
            case 3:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Обрыв") {
                        mdl->setMdlDt(ind, "Обрыв");
                        writeLog("Обрыв", ch, cl, true);
                    }
                }
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (_comm.param[numByte] & 3) {
            case 0:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::red)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Восстановление") {
                        mdl->setMdlDt(ind, "Восстановление");
                        writeLog("Восстановление", ch, cl, true);
                    }
                    else
                        mdl->setMdlDt(ind, "");
                }
                break;
            case 2:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "КЗ") {
                        mdl->setMdlDt(ind, "КЗ");
                        writeLog("КЗ", ch, cl, true);
                    }
                }
                break;
            case 3:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Обрыв") {
                        mdl->setMdlDt(ind, "Обрыв");
                        writeLog("Обрыв", ch, cl, true);
                    }
                }
                break;
            default:
                break;
            }
            mdl->setMdlDt(r, 9, translateTrubachMode((_comm.param[numByte] & 240) >> 4));
            break;
        case 3:
            if(currAnsw.param[0] & 128) {
                Command comm;
                unsigned char parAddress = currAnsw.param[0];
                comm.setCommand(currAnsw.numClass,
                                   parAddress,
                                   0x0E,
                                   &parAddress,
                                   1);
                commLst.push_front(comm);
            }
            switch (_comm.param[numByte] & 255) {
            case 0:
                mdl->setMdlDt(ind, "");
                break;
            case 1:
                mdl->setMdlDt(ind, "Нажата\nкнопка 1\n(Тревога)");
                break;
            case 2:
                mdl->setMdlDt(ind, "Нажата\nкнопка 2\n(Взять)");
                break;
            case 4:
                mdl->setMdlDt(ind, "Нажата\nкнопка 3\n(Снять)");
                break;
            case 8:
                mdl->setMdlDt(ind, "Нажата\nкнопка 4\n(Тест)");
                break;
            case 81:
                mdl->setMdlDt(ind, "Длительное\nнажатие\nкнопки 1\n(Тревога)");
                break;
            case 82:
                mdl->setMdlDt(ind, "Длительное\nнажатие\nкнопки 2\n(Взять)");
                break;
            case 84:
                mdl->setMdlDt(ind, "Длительное\nнажатие\nкнопки 3\n(Снять)");
                break;
            case 88:
                mdl->setMdlDt(ind, "Длительное\nнажатие\nкнопки 4\n(Тест)");
                break;
            default:
                mdl->setMdlDt(ind, "Неизвестный\nкод\nнажатой\nкнопки");
                break;
            }
            break;
        case 4:
            switch (_comm.param[numByte] & 16) {
            case 0:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::red)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Восстановление") {
                        mdl->setMdlDt(ind, "Восстановление");
                        writeLog("Восстановление", ch, cl, true);
                    }
                    else
                        mdl->setMdlDt(ind, "");
                }
                break;
            case 1:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Тревога\nпо верхнему\nпорогу") {
                        mdl->setMdlDt(ind, "Тревога\nпо верхнему\nпорогу");
                        writeLog("Тревога\nпо верхнему\nпорогу", ch, cl, true);
                    }
                }
                break;
            case 2:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Тревога\nпо нижнему\nпорогу") {
                        mdl->setMdlDt(ind, "Тревога\nпо нижнему\nпорогу");
                        writeLog("Тревога\nпо нижнему\nпорогу", ch, cl, true);
                    }
                }
                break;
            case 3:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Сброс\nдатчика") {
                        mdl->setMdlDt(ind, "Сброс\nдатчика");
                        writeLog("Сброс\nдатчика", ch, cl, true);
                    }
                }
                break;
            case 4:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Обрыв") {
                        mdl->setMdlDt(ind, "Обрыв");
                        writeLog("Обрыв", ch, cl, true);
                    }
                }
                break;
            case 5:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Нет\nпитания\nдатчика") {
                        mdl->setMdlDt(ind, "Нет\nпитания\nдатчика");
                        writeLog("Нет\nпитания\nдатчика", ch, cl, true);
                    }
                }
                break;
            case 6:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Замыкание\nна общий") {
                        mdl->setMdlDt(ind, "Замыкание\nна общий");
                        writeLog("Замыкание\nна общий", ch, cl, true);
                    }
                }
                break;
            case 7:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Замыкание\nмежду\nканалами") {
                        mdl->setMdlDt(ind, "Замыкание\nмежду\nканалами");
                        writeLog("Замыкание\nмежду\nканалами", ch, cl, true);
                    }
                }
                break;
            case 11:
                if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Нет\nкорректных\nданных") {
                        mdl->setMdlDt(ind, "Нет\nкорректных\nданных");
                        writeLog("Нет\nкорректных\nданных", ch, cl, true);
                    }
                }
                break;
            default:
                if(((_comm.param[numByte] & 16) > 7) && ((currAnsw.param[numByte] & 16) < 11) &&
                        (mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white))) {
                    mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                    if(mdl->mdlDt(ind).toString() != "Нет\n корректные\nданных") {
                        mdl->setMdlDt(ind, "Нет\nкорректных\nданных");
                        writeLog("Нет\nкорректных\nданных", ch, cl, true);
                    }
                }
                else
                    if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                        mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                        if(mdl->mdlDt(ind).toString() != "Некорректные\nданные") {
                            mdl->setMdlDt(ind, "Некорректные\nданные");
                            writeLog("Некорректные\nданные", ch, cl, true);
                        }
                    }
                break;
            }
            break;
        }
        break;
    case 1:
    {
        if(_comm.param[numByte] & (int)pow(2, bitVal)) {
            if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::white)) {
                mdl->setMdlDt(ind, QColor(Qt::red), Qt::BackgroundColorRole);
                if(!mess1.isEmpty()) {
                    mdl->setMdlDt(ind, mess1);
                    writeLog(mess1, ch, cl, true);
                }
            }
        }
        else {
            if(mdl->mdlDt(ind, Qt::BackgroundColorRole) == QColor(Qt::red)) {
                mdl->setMdlDt(ind, QColor(Qt::white), Qt::BackgroundColorRole);
                if(!mess1.isEmpty()) {
                    mdl->setMdlDt(ind, mess2);
                    writeLog(mess2, ch, cl, true);
                }
            }
            else {
                if(!mdl->mdlDt(ind).toString().isEmpty())
                    mdl->setMdlDt(ind, "");
            }
        }
        QString commonStrState = "Канал: ";
        bool redColorFlag = false;
        if(!mdl->mdlDt(r, 31).toString().isEmpty())
            commonStrState.append(mdl->mdlDt(r, 31).toString() + '\n');
        if(!mdl->mdlDt(r, 22).toString().isEmpty()) {
            commonStrState.append(mdl->mdlDt(r, 22).toString() + '\n');
            if(mdl->mdlDt(r, 22, Qt::BackgroundColorRole) == QColor(Qt::red))
                redColorFlag = true;
        }
        if(!mdl->mdlDt(r, 23).toString().isEmpty()) {
            commonStrState.append(mdl->mdlDt(r, 23).toString() + '\n');
            if(mdl->mdlDt(r, 23, Qt::BackgroundColorRole) == QColor(Qt::red))
                redColorFlag = true;
        }
        if(cl != 3) {
            if(!mdl->mdlDt(r, 24).toString().isEmpty()) {
                commonStrState.append(mdl->mdlDt(r, 24).toString());
                if(mdl->mdlDt(r, 24, Qt::BackgroundColorRole) == QColor(Qt::red))
                    redColorFlag = true;
            }
        }
        if(commonStrState.size() > 9) {
            if(redColorFlag)
                mdl->setMdlDt(r, 3, QColor(Qt::red), Qt::BackgroundColorRole);
            else
                mdl->setMdlDt(r, 3, QColor(Qt::white), Qt::BackgroundColorRole);
            mdl->setMdlDt(r, 3, commonStrState);
        }
        else
            mdl->setMdlDt(r, 3, "");
        //writeLog(commonStrState, ch, cl, true);
    }
    break;
    case 2:
        if(_comm.param[numByte] & (int)pow(2, bitVal)) {
            if(mdl->mdlDt(ind).toString() != mess1) {
                if(!ui->pbCancel->isEnabled())
                    mdl->setMdlDt(ind, mess1);
            }
        }
        else {
            if(mdl->mdlDt(ind).toString() != mess2) {
                if(!ui->pbCancel->isEnabled())
                    mdl->setMdlDt(ind, mess2);
            }
        }
        break;
    case 3:
        mdl->setMdlDt(ind, _comm.param[numByte]);
        break;
    default:
        break;
    }
    if(currComm.command == 0x69)
        addSpecCommands(r);
}

void MainWindow::parce0x69()
{
    writeInLogFlag = true;
    for(int i = 0; i < mdl->rowCount(); i++) {
        if(mdl->mdlDt(i, 3, Qt::BackgroundColorRole) == QColor(Qt::white)
                && !mdl->mdlDt(i, 3).toString().isEmpty())
            mdl->setMdlDt(i, 3, "");
    }
    int bit0 = currAnsw.param[0] & 1;
    int bit1 = currAnsw.param[0] & 128;
    if(bit0) {
        if(bit1) {
            if(lblNoize->text() != "Помеха\nсильная") {
                lblNoize->setText("Помеха\nсильная");
                lblNoize->setStyleSheet("QLabel { background-color : red; }");
                //writeLog("Помеха сильная", -1, -1, ui->chckBxNoize->isChecked());
            }
        }
        else {
            if(lblNoize->text() != "Помеха\n слабая") {
                lblNoize->setText("Помеха\nслабая");
                lblNoize->setStyleSheet("QLabel { background-color : green; }");
                //writeLog("Помеха слабая", -1, -1, ui->chckBxNoize->isChecked());
            }
        }
    }
    else {
        if(bit1) {
            if(lblNoize->text() != "Помеха\nсредняя") {
                lblNoize->setText("Помеха\nсредняя");
                lblNoize->setStyleSheet("QLabel { background-color : yellow; }");
                //writeLog("Помеха средняя", -1, -1, ui->chckBxNoize->isChecked());
            }
        }
        else {
            if(lblNoize->text() != "Помехи\nнет") {
                lblNoize->setText("Помехи\nнет");
                lblNoize->setStyleSheet("QLabel { background-color : white; }");
                //writeLog("Помехи нет", -1, -1, ui->chckBxNoize->isChecked());
            }
        }
    }
    if(currAnsw.param[0] & 64) {
        if(lblCoverStat->text() != "Вскрытие")
            writeLog("Вскрытие БРШС", -1, -1, true);
        lblCoverStat->setText("Вскрытие");
        lblCoverStat->setStyleSheet("QLabel { background-color : red; }");
    }
    else {
        if(lblCoverStat->text() == "Вскрытие") {
            lblCoverStat->setText("Вскрытие\nвосстановлено");
            lblCoverStat->setStyleSheet("QLabel { background-color : white; }");
            writeLog("Вскрытие БРШС восстановлено", -1, -1, true);
        }
        else
            lblCoverStat->setText(" ");
    }
    if(currAnsw.param[1] & 8) {
        if(lblBindStat->text() != "Связывание") {
            lblBindStat->setText("Связывание");
            lblBindStat->setStyleSheet("QLabel { background-color : red; }");
        }
    }
    else {
        if(lblBindStat->text() != "Связывания\nнет") {
            lblBindStat->setText("Связывания\nнет");
            lblBindStat->setStyleSheet("QLabel { background-color : white; }");
        }
    }
    if(currAnsw.param[0] & 4)
        lblIn1->setStyleSheet("QLabel { background-color : white; }");
    else
        lblIn1->setStyleSheet("QLabel { background-color : red; }");
    if(currAnsw.param[0] & 2)
        lblIn2->setStyleSheet("QLabel { background-color : white; }");
    else
        lblIn2->setStyleSheet("QLabel { background-color : red; }");
    if(currAnsw.nParam > 2) {
        Command paramPack;
        int k = 1;
        int size0x01Pack = 3;
        unsigned char tempPar[size0x01Pack];
        while(currAnsw.nParam - k >= 5) {
            qDebug() << "currAnsw.nParam - k =" << currAnsw.nParam - k;
            for(int i = 0; i < size0x01Pack; i++)
                tempPar[i] = currAnsw.param[i + k + 2];
            paramPack.setCommand(currAnsw.param[k],
                                 currAnsw.param[k + 1],
                                 0x01,
                                 tempPar,
                                 size0x01Pack);
            if(currAnsw.param[k + 1] < 1)
                return;
            parceParam0x01(paramPack);
            k += 5;
        }
    }
    if(chckBxInterView->isChecked())
        pb->wait(1000);
}

void MainWindow::parce0x61()
{
    QString slabel;
    unsigned char a = currAnsw.param[1];
    unsigned char b = currAnsw.param[0];
    int sn = ((int)a << 8) + (int)b;
    if(sn < 0xFFFF)
        slabel += QString::number(sn);
    else
        slabel += "-----";
    lblSN->setText("S/N:\n" + slabel);
    slabel.clear();
    if(currAnsw.param[2] < 10)
        slabel += QString::number(currAnsw.param[3]) + ".0"
                + QString::number(currAnsw.param[2]);
    else
        slabel += QString::number(currAnsw.param[3]) + "."
                + QString::number(currAnsw.param[2]);
    lblPOVer->setText("Версия прошивки:\n" + slabel);
}

void MainWindow::parce0x0c()
{
    int bpi, sni;
    QString conPar = "Период связывания: ";
    bpi = (currAnsw.param[0] & 15) - 1;
    if(bpi < 0)
        bpi = 0;
    sni = (currAnsw.param[0] & 240) >> 4;
    conPar.append(convertBindPer(bpi) + "\nДопустимое число\nпропусков: ");
    conPar.append(QString::number(sni) + "\nПериод контроля: ");
    int periodSec;
    switch (bpi) {
    case 0:
        periodSec = 10;
        break;
    case 1:
        periodSec = 15;
        break;
    case 2:
        periodSec = 30;
        break;
    case 3:
        periodSec = 60;
        break;
    case 4:
        periodSec = 300;
        break;
    case 5:
        periodSec = 600;
        break;
    case 6:
        periodSec = 120;
        break;
    default:
        break;
    }
    QString per = SetNewNet::calcPer(periodSec, sni);
    conPar.append(per);
    mdl->setMdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 28, periodSec);
    mdl->setMdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 29, sni);
    mdl->setMdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 30, per);
    mdl->setMdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 5, conPar);
}

QString MainWindow::convertBindPer(int indBindPer)
{
    return slTC.at(indBindPer);
}

void MainWindow::parce0x6c()
{
    int bpi, sni;
    netNum = currAnsw.param[0];
    numLiter = currAnsw.param[1];
    lblNetNum->setText("Сеть:\n" + QString::number(netNum));
    lblNumLiter->setText("Литера:\n" + QString::number(numLiter));
    if(netNum == 0 && numLiter == 0) {
        bindPeriodInd = 2;
        skipNum = 5;
        slotSetParams();
    }
    bpi = (currAnsw.param[2] & 15) - 1;
    sni = (currAnsw.param[2] & 240) >> 4;
    bindPeriodInd = bpi;
    skipNum = sni;
    controlPeriod = SetNewNet::calcPer(bpi, sni);
}

void MainWindow::parce0x0e()
{
    int numInd = mdl->rowMdl(currComm.numCh, currComm.numClass);
    QVariant blockData = mdl->mdlDt(numInd, 0);
    QString delMessage = "Удалён сетевой адрес: " + blockData.toString();
    if(mdl->mdlDt(numInd, 51).toInt() > 1)
        delMessage.append(", Каналы: ");
    else
        delMessage.append("Канал: ");
    for(int i = 0; i < mdl->rowCount(); i++) {
        if(mdl->mdlDt(i, 0) == blockData) {
            delMessage.append(mdl->mdlDt(i, 31).toString() + ",");
            mdl->removeRows(i, 1);
            i--;
        }
    }
    writeLog(delMessage, -1, currComm.numClass, true);
    for(int i = 0; i < 5; i++)
        arrOccupiedChans[i].clear();
    unsigned char prm[2];
    prm[0] = 0xFF;
    prm[1] = 0xFF;
    Command comm(currComm.numClass, -1, 0x15, prm, 2);
    commLst.push_back(comm);
}

QString MainWindow::translateTrubachMode(int par)
{
    //if(psb->confState)
    //    return;
    QString strPar;
    switch (par) {
    case 0:
        strPar = "выключено";
        break;
    case 1:
        strPar = "включено";
        break;
    case 2:
        strPar = "режим\n0.5 Гц";
        break;
    case 3:
        strPar = "режим\n1 Гц";
        break;
    case 4:
        strPar = "режим\n2 Гц";
        break;
    case 5:
        strPar = "режим\n4 Гц";
        break;
    default:
        strPar = QString::number(par);
        break;
    }
    return strPar;
}

void MainWindow::slotSetParams()
{
    SetNewNet *pSNN = new SetNewNet(broadcast_adr, netNum, numLiter, skipNum, bindPeriodInd, slTC);
    if(pSNN->exec() == pSNN->Accepted) {
        netNum = pSNN->getNetNum();
        numLiter = pSNN->getLiter();
        lblNetNum->setText("Сеть:\n" + QString::number(netNum));
        lblNumLiter->setText("Литера:\n" + QString::number(numLiter));
        skipNum = pSNN->getSkip();
        bindPeriodInd = pSNN->getBindPeriod().toInt();
        controlPeriod = pSNN->getContrPer();
        broadcast_adr = pSNN->getSysAddress();
        lblSysAddress->setText("Системный\nадрес: " + QString::number(broadcast_adr));
        create0x6d(netNum, numLiter, bindPeriodInd, skipNum);
    }
    delete pSNN;
}

void MainWindow::slotChangeDevNetSettings()
{
    SetNewNet *pSNN = new SetNewNet(skipNum, bindPeriodInd, slTC);
    if(pSNN->exec() == pSNN->Accepted) {
        skipNum = pSNN->getSkip();
        bindPeriodInd = pSNN->getBindPeriod().toInt();
        switch (bindPeriodInd) {
        case 4:
            bindPeriodInd = 6;
            break;
        case 5:
            bindPeriodInd = 4;
            break;
        case 6:
            bindPeriodInd = 5;
            break;
        default:
            break;
        }
        controlPeriod = pSNN->getContrPer();
        create0x0d(currTab, tbl->ind.row(), bindPeriodInd, skipNum);
    }
    delete pSNN;
}

void MainWindow::slotHelp()
{
    QString name = QString("help.chm").arg(QCoreApplication::applicationDirPath());
    if(!QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(name))))
        QMessageBox::critical(this, "Error", "Файл справки не найден", QMessageBox::Yes);
}

void MainWindow::parce0x03()
{
    /*if((currAnsw.param[4] == (dialBind->getBindPer().toInt() << 1 + dialBind->getVer())) &&
            (currAnsw.param[5] == dialBind->getTypeDev())) {*/
    writeLog("Привязано ОУ", currComm.numClass, currComm.numCh, true);
    if(!arrOccupiedChans[dialBind->getNumCls()].contains(addrBindChannel))
        arrOccupiedChans[dialBind->getNumCls()].append(addrBindChannel);
    parce0x14();
    dialBind->stopBindSlot();
    countBinds--;
    int cls = dialBind->getNumCls();
    slotBindSS(countBinds);
    arrFreeChans[cls].clear();
    for(int j = 1; j < numModelRows; j++) {
        if(!arrOccupiedChans[cls].contains(j))
            arrFreeChans[cls].append(j);
    }
    dialBind->setArrFreeChanVec(arrFreeChans[cls]);
    //}
}

void MainWindow::create0x0d(int cls, int row, int bindPer, int skipContr)
{
    unsigned char prm[2];
    prm[0] = row;
    prm[1] = skipContr << 4 + bindPer + 1;
    Command comm(cls, row, 0x0d, prm, 2);
    commLst.push_front(comm);
}

void MainWindow::slotTimeout0x01()
{
    Command comm;
    unsigned char parAddress;
    if(commLst.last().command != 0x03) {
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < arrOccupiedChans[i].size(); j++) {
                parAddress = arrOccupiedChans[i].at(j);
                comm.setCommand(i,
                                parAddress,
                                0x01,
                                &parAddress,
                                1);
                if(chckBxInterView->isChecked())
                    commLst.push_back(comm);
            }
        }
    }
    QTimer::singleShot(60000, this, SLOT(slotTimeout0x01()));
}

void MainWindow::parce0x0a()
{
    int row = mdl->rowMdl(currComm.numCh, currComm.numClass);
    int cls = mdl->mdlDt(row, 35).toInt();
    int ver = mdl->mdlDt(row, 39).toInt();
    int type = mdl->mdlDt(row, 43).toInt();
    int feature = mdl->mdlDt(row, 47).toInt();
    switch (cls) {
    case 1:
    {
        QString s;
        if(((ver == 4) && (type == 31) && (feature == 20)) || (cls == 2)) {
            if(currComm.param[currComm.nParam - 1] >> 7)
                s.append(QString("-"));
            else
                s.append(QString("+"));
            int t = (((currComm.param[currComm.nParam - 1] >> 4) & 0x7) * 1000)
                    + ((currComm.param[currComm.nParam - 1] & 0xF) * 100) +
                    ((currComm.param[currComm.nParam] >> 4) * 10)
                    + ((currComm.param[currComm.nParam] & 0xF));
            s.append(QString::number(t));
            mdl->setMdlDt(row, 18, s);
        }
        else {
            switch (currComm.param[0] & 3) {
            //case 0:
                //mdl->setMdlDt(row, 4, "Норма");
                //break;
            case 1:
                mdl->setMdlDt(row, 4, "Нарушен");
                break;
            case 2:
                mdl->setMdlDt(row, 4, "КЗ");
                break;
            case 3:
                mdl->setMdlDt(row, 4, "Обрыв");
                break;
            default:
                break;
            }
            mdl->setMdlDt(row, 4, s);
        }
        break;
    }
    case 3:
    {
        QString data4a;
        for(int i = 0; i < currComm.param[currComm.nParam] - 1; i++)
            data4a.append("0x" + QString::number(currComm.param[1 + i]) + " ");
        mdl->setMdlDt(row, 18, data4a);
    }
    case 4:
    {
        unsigned char par1b = currComm.param[0];
        unsigned char par2b = currComm.param[1];
        int e, d, res;
        d = par1b << 8;
        e = par2b;
        res = d | e;
        QString aDataStr;
        if(par1b & 0x80) {
            res -= 0x8000;
            aDataStr = "-" + QString::number(res, 16);
        }
        else
            aDataStr = QString::number(res, 16);
        if(aDataStr.toInt() != -1)
        {
            if((aDataStr.toInt() < 125) && (aDataStr.toInt() > -55))
                mdl->setMdlDt(row, 18, aDataStr);
            else
            {
                switch (aDataStr.toInt()) {
                case -7910:
                    mdl->setMdlDt(row, 18, "Reset");
                    writeLog("Reset", currComm.numCh, currComm.numClass, true);
                    break;
                case -7911:
                    mdl->setMdlDt(row, 18, SignalErr7911);
                    writeLog(SignalErr7911, currComm.numCh, currComm.numClass, true);
                    break;
                case -7912:
                    mdl->setMdlDt(row, 18, SignalErr7912);
                    writeLog(SignalErr7912, currComm.numCh, currComm.numClass, true);
                    break;
                case -7913:
                    mdl->setMdlDt(row, 18, SignalErr7913);
                    writeLog(SignalErr7913, currComm.numCh, currComm.numClass, true);
                    break;
                case -7914:
                    mdl->setMdlDt(row, 18, SignalErr7914);
                    writeLog(SignalErr7914, currComm.numCh, currComm.numClass, true);
                    break;
                case -7915:
                    mdl->setMdlDt(row, 18, SignalErr7915);
                    writeLog(SignalErr7915, currComm.numCh, currComm.numClass, true);
                    break;
                case -7916:
                    mdl->setMdlDt(row, 18, SignalErr7916);
                    writeLog(SignalErr7916, currComm.numCh, currComm.numClass, true);
                    break;
                case -7917:
                    mdl->setMdlDt(row, 18, SignalErr7917);
                    writeLog(SignalErr7917, currComm.numCh, currComm.numClass, true);
                    break;
                case -7918:
                    mdl->setMdlDt(row, 18, SignalErr7918);
                    writeLog(SignalErr7918, currComm.numCh, currComm.numClass, true);
                    break;
                case -7919:
                    mdl->setMdlDt(row, 18, SignalErr7919);
                    writeLog(SignalErr7919, currComm.numCh, currComm.numClass, true);
                    break;
                default:
                    qDebug() << "Unknown error";
                    break;
                }
            }
        }
        else
            qDebug() << "Unknown error aDataStr =" << aDataStr;
    }
    default:
        break;
    }
}

void MainWindow::parce0x12()
{
    int row = mdl->rowMdl(currComm.numCh, currComm.numClass);
    int cls = mdl->mdlDt(row, 35).toInt();
    int ver = mdl->mdlDt(row, 39).toInt();
    int type = mdl->mdlDt(row, 43).toInt();
    int feature = mdl->mdlDt(row, 47).toInt();
    switch (cls) {
    case 1: {
        int par = currComm.param[0];
        if(mdl->mdlDt(row, 51).toInt() == 1) {
            switch (par) {
            case 1:
                mdl->setMdlDt(row, 11, QString::number(par) + " - min");
                break;
            case 8:
                mdl->setMdlDt(row, 11, QString::number(par) + " - max");
                break;
            default:
                mdl->setMdlDt(row, 11, QString::number(par));
                break;
            }
        }
        else
            translateChannelOne(row, 11, par);
        break;
    }
    case 2: {
        int val1 = QString::number(currComm.param[0], 16).toInt() * 100;
        int val2 = QString::number(currComm.param[1], 16).toInt() * 100;
        mdl->setMdlDt(row, 16, QString::number(val1));
        mdl->setMdlDt(row, 17, QString::number(val2));
        break;
    }
    case 4:
    {
        unsigned char par1b = currComm.param[0];
        unsigned char par2b = currComm.param[1];
        unsigned char par3b = currComm.param[2];
        unsigned char par4b = currComm.param[3];
        int e1, e2, d1, d2, res1, res2;
        d1 = par1b << 8;
        e1 = par2b;
        res1 = d1 | e1;
        if(par1b & 0x80) {
            res1 -= 0x8000;
            mdl->setMdlDt(row, 12, "-" + QString::number(res1, 16));
        }
        else
            mdl->setMdlDt(row, 12, QString::number(res1, 16));
        mdl->setMdlDt(row, 12, QColor(Qt::white), Qt::BackgroundColorRole);
        d2 = par3b << 8;
        e2 = par4b;
        res2 = d2 | e2;
        if(par3b & 0x80) {
            res2 -= 0x8000;
            mdl->setMdlDt(row, 13, "-" + QString::number(res2, 16));
        }
        else
            mdl->setMdlDt(row, 13, QString::number(res2, 16));
        mdl->setMdlDt(row, 13, QColor(Qt::white), Qt::BackgroundColorRole);
    }
    break;
    default:
        break;
    }
}

void MainWindow::translateChannelOne(int row, int col, int par)
{
    switch (par) {
    case 0:
        par = 3;
        break;
    case 1:
        par = 10;
        break;
    case 2:
        par = 25;
        break;
    case 3:
        par = 45;
        break;
    default:
        break;
    }
    mdl->setMdlDt(row, col, QString::number(par) + QChar(176)/*QChar('°')*/);
}

void MainWindow::parceSetCommands()
{
    QString val = currComm.param[0] == 1 ? "Вкл." : "Выкл.";
    QString mess;
    switch (currComm.command) {
    case 0x19:
        mess = "Кодирование данных " + val;
        break;
    case 0x07:
        mess = "Индикация состояния " + val;
        break;
    case 0x02:
        mess = "Режим охраны " + val;
        break;
    case 0x06:
        mess = "Звуковая индикация " + val;
        break;
    case 0x08:
        mess = "Световая индикация " + val;
        break;
    default:
        mess = "Команда " + QString::number(currComm.command, 16);
        break;
    }
    writeLog(mess, currComm.numCh, currComm.numClass, true);
}

void MainWindow::parce0x13()
{
    QString mess;
    switch (currComm.numClass) {
    case 1:
        mess = "Установлены пороги срабатывания Грань-РК; ";
        if(mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 52).toInt() == 1)
            mess = "канал 1: " + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 11).toString();
        else
            mess = "канал 2: " + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 11).toString();
        writeLog(mess, currComm.numCh, currComm.numClass, true);
        break;
    case 2:
        writeLog("Назначены длительности фаз режима" + QString(currComm.param[0]) + "работы исполнительного ОУ;"
                + " активная фаза: " + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 16).toString()
                + " пассивная фаза: " + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 17).toString(),
                currComm.numCh, currComm.numClass, true);
        break;
    case 4:
        writeLog(QString("Установлены пороги срабатывания аналогового ОУ;")
                + " верхний:" + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 12).toString()
                + " нижний:" + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 13).toString(),
                 currComm.numCh, currComm.numClass, true);
        break;
    default:
        break;
    }
}

void MainWindow::parce0x29()
{
    writeLog("Выбран режим работы канала: " + QString(currComm.param[0]) + "; Время работы режима: "
            + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 15).toString(),
            currComm.numCh, currComm.numClass, true);
}

void MainWindow::parce0x0d()
{
    writeLog("Назначены параметры связи с ОУ:"
             + mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 5).toString(),
             currComm.numCh, currComm.numClass, true);
}
