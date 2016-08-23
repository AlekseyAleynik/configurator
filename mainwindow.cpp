#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QAbstractItemDelegate>
#include "abstractserial.h"
#include "dialogselectconnection.h"
#include "delegaterowcmbbx.h"
#include "delegaterowspnbx.h"

#define defaultData     "8 bit"
#define defaultParity   "None"
#define defaultStop     "1"
#define defaultFlow     "Disable"

#define SignalErr7911 "Не подключен датчик"
#define SignalErr7912 "Не подключено питание датчика"
#define SignalErr7913 "КЗ между каналом и землей"
#define SignalErr7914 "КЗ между двумя каналами"
#define SignalErr7915 "Ошибка времени преобразования"
#define SignalErr7916 "Отключение датчика после преобразования"
#define SignalErr7917 "Ошибка CRC"
#define SignalErr7918 "Ошибка конфигурации датчика"
#define SignalErr7919 "Считанная температура не находиться в диапазоне от -55 до +125"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    qEx = true;
    slTC << "10 сек" << "15 сек" << "30 сек" << "1 мин" << "5 мин" << "10 мин";
    answerFlag = true;
    len_pack = c_b = add = 0;
    global_dev_type = 0x06;
    broadcast_adr = 0x01;
    s_cnt_start_bytes = 1;
    s_num_c_com_byte = 5;
    s_num_s_com_byte = 5;
    baudList << "9600" << "57600" << "115200";
    pprotocol485RK = new Protocol485RK();
    //_settings.setValue("Settings/LastDatabase", "log/Base.db");
    _settings.setValue("Settings/LastMoxa", "USB\\VID_110A&PID_1150");
    _settings.setValue("Settings/LastBaud", baudList[1]);
    //lg = new Log(_settings.value("Settings/LastDatabase", "log/Base.db").toString());
    numBytes0x15com = 16;
    bootWGT = new BootWidget(this);
    connect(bootWGT, SIGNAL(signal_refresh()), this, SLOT(refresh()));
    connect(bootWGT, SIGNAL(setBaud(int)), this, SLOT(setBaudSlot(int)));
    connect(bootWGT, SIGNAL(sign6f()), this, SLOT(slot6fBootFirmware()));
    connect(bootWGT, SIGNAL(signal_SendPacket(QByteArray)), this, SLOT(on_signalSendData(QByteArray)));
    connect(ui->actLoad, SIGNAL(triggered(bool)), this, SLOT(slotActLoad()));
    broadcast_adr = 1;
    textCounterLog = 0;
    textCounterSwap = 0;
    bootFlag = false;
    refreshFlag = false;
    out_y = new QPixmap(":/images/out_y.JPG");
    out_g = new QPixmap(":/images/out_g.JPG");
    lblArrow = new QLabel();
    lblArrow->setPixmap(*out_y);
    pserial = new AbstractSerial();
    m_sde = SerialDeviceEnumerator::instance();
    availablePorts = m_sde->devicesAvailable();
    pb = new PortBaud();
    chckBxInterView = new QCheckBox();
    chckBxInterView->setChecked(true);
    initConnectParams();
    startFunctions();
}

MainWindow::~MainWindow()
{
    exit(0);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    exit(0);
}

void MainWindow::closeApp(QString exitText)
{
    QMessageBox *pemb = new QMessageBox(QMessageBox::Critical, "Ошибка", exitText, QMessageBox::Ok);
    pemb->exec();
    //delete pemb;
    this->close();
    QApplication::quit();
    qEx = false;
}

void MainWindow::startFunctions()
{
    headersLst.append("ОУ");
    headersLst.append("Адрес");
    headersLst.append("Класс");
    treeMdl = new TreeModel(headersLst);
    ui->progressBarTree->hide();
    lblSN = new QLabel;
    lblPOVer = new QLabel();
    lblNetNum = new QLabel();
    lblNumLiter = new QLabel();
    lblStatus = new QLabel();
    lblPort = new QLabel();
    lblSysAddress = new QLabel();
    lblCoverStat = new QLabel();
    lblBindStat = new QLabel();
    lblIn1 = new QLabel();
    lblIn2 = new QLabel();
    lblNoize = new QLabel();
    lblInterView = new QLabel();
    statusBar()->addWidget(lblArrow, 0);
    statusBar()->addWidget(lblStatus, 0);
    statusBar()->addWidget(lblPort, 0);
    statusBar()->addWidget(lblCoverStat, 0);
    statusBar()->addWidget(lblBindStat, 0);
    statusBar()->addWidget(lblIn1, 0);
    statusBar()->addWidget(lblIn2, 0);
    statusBar()->addWidget(lblNoize, 0);
    statusBar()->addWidget(lblPOVer, 0);
    statusBar()->addWidget(lblSN, 0);
    statusBar()->addWidget(lblNetNum, 0);
    statusBar()->addWidget(lblNumLiter, 0);
    statusBar()->addWidget(lblSysAddress, 0);
    statusBar()->addWidget(chckBxInterView, 0);
    statusBar()->addWidget(lblInterView, 0);
    lblBindStat->setFixedSize(61, 26);
    lblNoize->setFixedSize(40, 26);
    lblCoverStat->setFixedSize(77, 26);
    lblBindStat->setStyleSheet("QLabel { background-color : white; }");
    lblNetNum->setText("Сеть:\n");
    lblNetNum->setToolTip("Номер сети");
    lblNumLiter->setText("Литера:\n");
    lblNumLiter->setToolTip("Номер литеры");
    lblSN->setText("S/N:\n");
    lblSN->setToolTip("Серийный номер контроллера");
    lblSysAddress->setText("Системный\nадрес: ");
    lblSysAddress->setToolTip("Системный адрес контроллера");
    lblPOVer->setText("Версия прошивки:\n");
    lblPOVer->setToolTip("Версия прошивки контроллера:");
    lblInterView->setText("Опрос");
    connect(ui->actUpdate, SIGNAL(triggered()), this, SLOT(slotActUpdate()));
    connect(ui->actExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(ui->actSetNetParams, SIGNAL(triggered()), this, SLOT(slotSetParams()));
    connect(ui->actExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actAbout, SIGNAL(triggered()), this, SLOT(slotAboutApp()));
    connect(ui->actRestart, SIGNAL(triggered()), this, SLOT(slotReboot()));
    connect(ui->actConnectPort, SIGNAL(triggered()), this, SLOT(slotConnectTo()));
    connect(ui->pbBind, SIGNAL(clicked()), this, SLOT(slotBind()));
    connect(ui->actTree, SIGNAL(triggered()), this, SLOT(slotAskTree()));
    connect(chckBxInterView, SIGNAL(clicked(bool)), this, SLOT(slotInterview(bool)));
    connect(ui->pbSet, SIGNAL(clicked()), this, SLOT(slotSetSettings()));
    connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(slotCancelSettings()));
    connect(ui->act_showHideLog, SIGNAL(triggered(bool)), this, SLOT(slotShowHideLogAct(bool)));
    connect(ui->pbClear, SIGNAL(clicked(bool)), this, SLOT(clearTxtEdit(bool)));
    connect(ui->cmbBxCls, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeCls(int)));
    connect(ui->cmbBxCmnd, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeCmnd(QString)));
    connect(ui->pbSend, SIGNAL(clicked()), this, SLOT(slotPBSend()));
    connect(ui->pages, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));
    connect(ui->pbLoad, SIGNAL(clicked()), this, SLOT(slotAskTree()));
    connect(treeMdl, SIGNAL(signalDropTreeNode(int, int, int, int, unsigned char)), this,
            SLOT(slotDropTreeNode(int, int, int, int, unsigned char)));
    answerFlag = true;
    writeInLogFlag = false;
    customCommFlag = false;
    lblCoverStat->setText("Крышка\nзакрыта");
    lblBindStat->setText("Связывания\nнет");
    lblIn1->setText("Вход 1");
    lblIn2->setText("Вход 2");
    /*this->setMouseTracking(true);
    ui->tabs->setMouseTracking(true);*/
    createDev();
    loadCommLst();
    loadDevSpecMap();
    ui->treeView->setModel(treeMdl);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeView->setDragEnabled(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->expandAll();
    updateActions();
    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->addWidget(lgWgt);
    ui->logTab->setLayout(vbl);
    //connect(this, SIGNAL(signal_log_click()), logWgt, SLOT(slot_log_clicked()));
    qDebug() << "start";
    if(pb->pairObjArr.isEmpty())
        //closeApp("<u><b>Приложение будет закрыто.</b></u><i> Нет доступных подключений.</i>");
        qDebug() << "No connection";
    else {
        connect(m_sde, SIGNAL(hasChanged(QStringList)), this, SLOT(printDevices(QStringList)));
        if(pb->pairObjArr.size() > 1) {
            dsc = new DialogSelectConnection(pb);
            connect(dsc, SIGNAL(tryConnSignal(QString, QString)), this, SLOT(sendTryConn(QString, QString)));
            if(dsc->exec() != dsc->Accepted)
                closeApp("<u><b>Приложение будет закрыто.</b></u><i> Подключение не выбрано.</i>");
            delete dsc;
        }
        else {
            currPair.port = pb->pairObjArr.first().port;
            currPair.baud = pb->pairObjArr.first().baud;
            pserial = new AbstractSerial(this);
            if(!tryConnect(pserial, currPair.port, currPair.baud))
                qDebug() << "serial port is not open";
        }
        disconnect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(parceAnswerPack(QByteArray)));
        connect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(onPackReceiver(QByteArray)));
        connect(pserial, SIGNAL(readyRead()), this, SLOT(onProcSerialReceive()));
        pserial->enableEmitStatus(true);
        lblPort->setText(currPair.port + "\n" + currPair.baud);
        lblStatus->setText("Connected");
        //lblStatus->setBackgroundRole(QColor(Qt::green));
        MainWindow::setWindowTitle(currPair.port + " Конфигуратор Ладога-РК");
        commLst.clear();
        QTimer::singleShot(50, this, SLOT(createStartCommands()));
    }
}

void MainWindow::slotConnectTo()
{
    pserial->close();
    answerFlag = true;
    pb->delPair(currPair.port);
    availablePorts.clear();
    m_sde = SerialDeviceEnumerator::instance();
    availablePorts = m_sde->devicesAvailable();
    //if(!pserial->isOpen() || !pserial->isReadable() || !pserial->isWritable())
    initConnectParams();
    dsc = new DialogSelectConnection(pb);
    connect(dsc, SIGNAL(tryConnSignal(QString, QString)), this, SLOT(sendTryConn(QString, QString)));
    dsc->getFirstPort();
    disconnect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(parceAnswerPack(QByteArray)));
    connect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(onPackReceiver(QByteArray)));
    connect(pserial, SIGNAL(readyRead()), this, SLOT(onProcSerialReceive()));
    pserial->enableEmitStatus(true);
    lblPort->setText(currPair.port + "\n" + currPair.baud);
    lblStatus->setText("Connected");
    MainWindow::setWindowTitle(currPair.port + " Конфигуратор Ладога-РК");
    if(dsc->exec() == dsc->Accepted)
        createStartCommands();
    else
        if(headersLst.size() == 0)
            closeApp("<u><b>Приложение будет закрыто.</b></u><i> Подключение не выбрано.</i>");
}

void MainWindow::slotAboutApp()
{
    AboutDilog *abd = new AboutDilog();
    abd->setWindowTitle("О программе");
    abd->exec();
}

void MainWindow::setBaudSlot(int baud)
{
    if(baud)
        pserial->setBaudRate(QString::number(baud));
    else
        pserial->setBaudRate(_settings.value("Settings/LastBaud", baudList[1]).toString());
}

void MainWindow::createStartCommands()
{
    Command comm(-1, -1, 0x6c);
    commLst.push_front(comm);
    comm.setCommand(-1, -1, 0x61);
    commLst.push_front(comm);
    unsigned char prm[2];
    prm[0] = 0xFF;
    prm[1] = 0xFF;
    for(int i = 0; i < 5; i++) {
        comm.setCommand(i, -1, 0x15, prm, 2);
        commLst.push_front(comm);
    }
    QTimer::singleShot(60000, this, SLOT(slotTimeout0x01()));
    mainCycle();
}

void MainWindow::mainCycle()
{
    while(chckBxInterView->isChecked()) {
        bool flag69 = false;
        if(!commLst.isEmpty()) {
            if((commLst.last().time == 0) || (commLst.last().command == 0x03))
                sendPack(commLst.last());
            qDebug() << "commLst.last().command =" << QString::number(commLst.last().command, 16);
            for(int i = commLst.size() - 1; i >= 0 ; i--) {
                if((commLst.at(i).numCh > 0) && (commLst.at(i).counter > 0) && (commLst.at(i).command != 0x03)) {
                    Command comm(commLst.takeAt(i));
                    int sni, bni;
                    sni = mdl->mdlDt(mdl->rowMdl(comm.numCh, comm.numClass), 29).toInt();
                    bni = mdl->mdlDt(mdl->rowMdl(comm.numCh, comm.numClass), 28).toInt();
                    qDebug() << "comm.time =" << comm.time << "sni * bni =" << sni * bni;
                    //if(comm.time < (sni * bni)) {
                    if(((bni / 2) < 1) || (comm.time < (bni / 2))) {
                        comm.time = 0;
                        commLst.push_back(comm);
                        sendPack(commLst.last());
                    }
                    else {
                        comm.time--;
                        commLst.push_front(comm);
                        mdl->setMdlDt(mdl->rowMdl(comm.numCh, comm.numClass), 19,
                                      QString::number(comm.time / 2));
                    }
                    flag69 = true;
                }
            }
        }
        if(flag69 || commLst.isEmpty()) {
            Command comm(-1, -1, 0x69);
            commLst.push_front(comm);
            sendPack(comm);
        }
        if(!answerFlag) {
            qDebug() << "!answerFlag" << tryConnect(pserial, currPair.port, currPair.baud);
            pb->wait(1000);
            answerFlag = true;
            sendPack(commLst.last());
        }
        qApp->processEvents();
    }
    if(customCommFlag)
        sendCustomComm();
    if(refreshFlag)
        refresh();
}

void MainWindow::setAddr(int _addr)
{
    broadcast_adr = _addr;
}

void MainWindow::initConnectParams()
{
    QEventLoop eventLoop, eventLoop2;
    for(int i = 0; i < availablePorts.size(); i++) {
        for(int j = 0; j < baudList.size(); j++) {
            currPair.port = availablePorts.at(i);
            currPair.baud = baudList.at(j);
            pserial = new AbstractSerial(this);
            if(tryConnect(pserial, currPair.port, currPair.baud)) {
                answerFlag = true;
                connect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(parceAnswerPack(QByteArray)));
                connect(this, SIGNAL(signPackReceive(QByteArray)), &eventLoop2, SLOT(quit()));
                connect(pserial, SIGNAL(readyRead()), this, SLOT(onProcSerialReceive()));
                connect(pserial, SIGNAL(readyRead()), &eventLoop, SLOT(quit()));
                Command comm(-1, -1, 0x6c);
                sendPack(comm);
                QTimer::singleShot(100, &eventLoop, SLOT(quit()));
                eventLoop.exec();
                QTimer::singleShot(100, &eventLoop2, SLOT(quit()));
                eventLoop2.exec();
            }
            answerFlag = true;
            pserial->close();
        }
    }
}

bool MainWindow::sendTryConn(QString _port, QString _baud)
{
    currPair.port = _port;
    currPair.baud = _baud;
    pserial->close();
    pserial = new AbstractSerial(this);
    return tryConnect(pserial, _port, _baud);
}

bool MainWindow::tryConnect(AbstractSerial *_pser, QString _port, QString _baud)
{
    _pser->setDeviceName(_port);
    if(_pser->open(QIODevice::ReadWrite)) {
        _settings.setValue("Settings/LastBaud", _baud);
        bool result = _pser->setBaudRate(_baud);
        if(result && (_pser->dataBits() != defaultData))
            result = _pser->setDataBits(defaultData);
        if(result && (_pser->parity() != defaultParity))
            result = _pser->setParity(defaultParity);
        if(result && (_pser->stopBits() != defaultStop))
            result = _pser->setStopBits(defaultStop);
        if(result && (_pser->flowControl() != defaultFlow))
            result = _pser->setFlowControl(defaultFlow);
        if(result && _pser->isReadable())
            return result;
    }
    return false;
}

void MainWindow::parceAnswerPack(QByteArray ba)
{
    if(ba.at(0) == char(0xC1) || ba.at(0) == char(0xC2))
        //pb->appendPair(pserial->deviceName(), pserial->baudRate().remove(QRegExp("\D*")));
        pb->appendPair(currPair.port, currPair.baud);
}

void MainWindow::boot_connect()
{
    //delete pserial;
    pserial->close();
    pserial = new AbstractSerial(this);
    pserial->enableEmitStatus(true);
    tryConnect(pserial, currPair.port, currPair.baud);
    connect(pserial, SIGNAL(readyRead()), this, SLOT(onProcSerialReceive()));
}

void MainWindow::sendPack(Command &_comm)
{
    answerFlag = false;
    if(!pserial->isWritable())
        return;
    unsigned char repeatByte = 0;
    if(_comm.counter > 0)
        repeatByte = 0x80;
    unsigned char cmnd;
    unsigned char *cmndParam;
    int numCmndParam = _comm.nParam;
    if(!_comm.numClass) {
        cmndParam = new unsigned char[++numCmndParam];
        cmndParam[0] = _comm.command;
        cmnd = 0x7f;
        for(int i = 0; i < numCmndParam; i++)
            cmndParam[i + 1] = _comm.param[i];
    }
    else {
        cmndParam = new unsigned char[numCmndParam];
        cmndParam = _comm.param;
        cmnd = _comm.command;
        if(_comm.numClass != -1) {
            cmnd += (_comm.numClass - 1) * 0x20;\
            if(_comm.numClass == 4)
                cmnd += 0x20;
        }
    }
    QByteArray a = pprotocol485RK->MakeRequest((unsigned char)global_dev_type,
                                          (unsigned char)broadcast_adr,
                                          (unsigned char)repeatByte,
                                          &cmnd,
                                          cmndParam,
                                          (unsigned char)numCmndParam);
    //if(!snifferMode)
        //pserial->write(a);
    //connect(this, SIGNAL(signPackReceive(QByteArray)), &eventLoop, SLOT(quit()));
    if(pserial->write(a) != -1) {
        QString txt;
        for(int i = 0; i < a.size(); i++)
            txt.append("0x" + QString::number(a.at(i), 16) + " ");
        qDebug() << "txt =" << txt;
        currComm.setCommand(_comm);
        addInTxtEditSwap(a);
    }
    else {
        if(!pserial->isReadable() || !pserial->isWritable()) {
            pserial->close();
            answerFlag = true;
            qDebug() << "reconnect()";
            if(!sendTryConn(currPair.port, currPair.baud))
                pb->wait(1000);
        }
    }
    if(currComm.command == 0x6f) {
        pb->wait(1000);
        return;
    }
    if(customCommFlag && ui->spnBxMultiplicator->value() && ui->chkBxTimer->isChecked())
        pb->wait(100 * ui->spnBxMultiplicator->value());
    else
        pb->wait(80);
    onProcSerialReceive();
    //QEventLoop eventLoop;
    //QTimer::singleShot(100, &eventLoop, SLOT(quit()));
    //eventLoop.exec();
    //if(!answerFlag)
        qApp->processEvents();
}

void MainWindow::onProcSerialReceive()
{
    qint64 ba;
    if((ba = pserial->bytesAvailable()) > 0) {
        lblArrow->clear();
        lblArrow->setPixmap(*out_g);
        QByteArray stream = pserial->readAll();
        if(!bootFlag)
            compilAnswer(ba, stream);
        else
            emit signPackReceive(stream);
    }
}

void MainWindow::compilAnswer(qint64 ba, QByteArray a)
{
    char zero_byte = 0x00;
    bool flag_len_temp = false;
    int j = 0, k = 0, start;
    if(s_cnt_start_bytes == 1) {
        int parSize, startByte;
        tempByteArr.append(a);
        startByte = tempByteArr.indexOf(0xC2);
        if(startByte >= 0) {
            if(startByte != tempByteArr.size()) {
                if((tempByteArr.at(startByte + 1) == 0x06) && ((startByte + 7) < tempByteArr.size())) {
                    parSize = tempByteArr.at(startByte + 4);
                    answ_line.clear();
                    answ_line.append(tempByteArr.right(tempByteArr.size() - startByte));
                    int icx = answ_line.indexOf(0xC0, 4);
                    answ_line = answ_line.left(7 + parSize);
                    if(answ_line.size() >= (7 + parSize)) {
                        if(answ_line.contains(0xC0)) {
                            while((icx >= 0) && (answ_line.size() > icx) && (answ_line.at(icx + 1) >= 0)
                                    && (answ_line.at(icx + 1) <= 2) && (icx < (answ_line.size() - 1))) {
                                answ_line.remove(icx, 1);
                                answ_line.insert(icx, 0xC0 + answ_line.at(icx));
                                answ_line.remove(icx + 1, 1);
                                answ_line.remove(4, 1);
                                answ_line.insert(4, ++parSize);
                                answ_line.append(tempByteArr.at(startByte + 7 + parSize));
                                icx = answ_line.indexOf(0xC0, icx + 1);
                            }
                        }
                        tempByteArr.clear();
                        if((answ_line.at(1) == 0x06) && ((answ_line.at(2) > 0) || (answ_line.at(2) < 4))) {
                            addInTxtEditSwap(answ_line);
                            emit signPackReceive(answ_line);
                        }
                    }
                    /*else {
                        pb->wait(30);
                        onProcSerialReceive();
                    }*/
                }
                else {
                    if(((startByte + 1) < tempByteArr.size()) && (tempByteArr.at(startByte + 1) != 0x06)) {
                        tempByteArr.clear();
                        answ_line.clear();
                        startByte = -1;
                    }
                    /*else {
                        pb->wait(30);
                        onProcSerialReceive();
                    }*/
                }
            }
            /*else {
                pb->wait(30);
                onProcSerialReceive();
            }*/
        }
    }
    else {
        if(s_cnt_start_bytes == 2) {
            start = a.indexOf(0xb9, 0);
            if((start != -1) && ((a[start] == 0x46) || (a[start + 1] == 0x46))) {
                answ_line.clear();
                add = 0;
                c_b = 0;
                len_pack = 0;
                j = start + 1;
                if(a[start] == 0x46) {
                    answ_line.push_back(0xb9);
                    c_b++;
                }
            }
            k = a.indexOf(0xb9, j);
            while(k != -1) {
                j = k + 1;
                if(j <= a.length()) {
                    if(a[j] == zero_byte) {
                        add++;
                        a.remove(j, 1);
                    }
                    k = a.indexOf(0xb9, j + 1);
                }
            }
            c_b += ba;
            answ_line.append(a);
            if(!flag_len_temp) {
                if(answ_line.length() >= 6) {
                    flag_len_temp = true;
                    len_pack = (int)(answ_line.at(5)) + 8;
                }
            }
            if(c_b == (add + len_pack)) {
                c_b = 0;
                add = 0;
                addInTxtEditSwap(answ_line);
                emit signPackReceive(answ_line);
            }
        }
    }
}

void MainWindow::onPackReceiver(QByteArray answ_line)
{
    if(answ_line.isEmpty())
        return;
    QString txt;
    for(int i = 0; i < answ_line.size(); i++)
        txt.append("0x" + QString::number(answ_line.at(i), 16) + " ");
    qDebug() << "answer =" << txt;
    lblStatus->setText("Connected");
    answerFlag = true;
    int countParByteAnsw = answ_line[s_num_c_com_byte - 1] - 1;
    unsigned char commAnsw = answ_line[s_num_c_com_byte];
    unsigned char parAnsw[countParByteAnsw];
    if(countParByteAnsw) {
        for(int i = 0; i < countParByteAnsw; i++)
            parAnsw[i] = answ_line[i + 1 + s_num_c_com_byte];
    }
    else
        parAnsw[0] = 0;
    currAnsw.setCommand(currComm.numClass,
                        currComm.numCh,
                        commAnsw,
                        parAnsw,
                        countParByteAnsw,
                        currComm.counter);
    qDebug() << "countParByteAnsw =" << countParByteAnsw;
    if(!commLst.isEmpty() && (commLst.last().time < 1))
        commLst.pop_back();
    switch (commAnsw) {
    case 0x07:
        if(currComm.command != 0x03)
            if(currComm.command == 0x0e)
                parce0x0e();
            else
                currComm.time = currAnsw.getDelay();
        else {
            qDebug() << "bintimeout";
            pb->wait(1000);
            emit bindTimeout();
        }
        currComm.counter++;
        qDebug() << "currComm.time =" << currComm.time << "currComm.counter =" << currComm.counter;
        if((currComm.time == 0) && (currComm.command != 0x0e) && (!bindStopFlag))
            commLst.push_back(currComm);
        if((currComm.command != 0x0e) && (currComm.command != 0x03) && (currComm.counter == 1))
            commLst.push_front(currComm);
        if(bindStopFlag)
            bindStopFlag = false;
        break;
    case 0x00:
        parceAnsw();
        break;
    default:
        parceErrorAnswer();
        break;
    }
}

void MainWindow::addInTxtEditSwap(QByteArray baPack)
{
    textCounterSwap++;
    if(textCounterSwap > 1000) {
        textCounterSwap = 0;
        ui->txtEditSwap->clear();
    }
    int sizePack = baPack.size();
    if(sizePack <= 0)
        return;
    QString strMess = QTime::currentTime().toString("hh:mm:ss.zzz");
    unsigned char firstByte = baPack.at(0);
    if(firstByte == 0xC2)
        strMess.append(" >>> \n");
    else
        strMess.append(" <<< \n");
    QString strByte = QString(baPack.toHex());
    for(int i = 0; i < (strByte.size() / 2); i++) {
        strMess.append(strByte.at(i * 2));
        strMess.append(strByte.at(i * 2 + 1));
        strMess.append(' ');
    }
    ui->txtEditSwap->append(strMess + "\n");
}

void MainWindow::parceErrorAnswer()
{

    switch (currAnsw.command) {
    case 0x01:
        qDebug() << "err 0x01: CRC error";
        break;
    case 0x02:
        qDebug() << "err 0x02: Command impossible";
        break;
    case 0x04:
        qDebug() << "err 0x04: CRC unsupport";
        break;
    case 0x05:
        qDebug() << "err 0x05: Binding error";
        break;
    case 0x06:
        qDebug() << "err 0x06: Channel busy";
        break;
    case 0x08:
        qDebug() << "err 0x08: Params unset";
        break;
    case 0x09:
        qDebug() << "err 0x09: Channel error";
        break;
    default:
        qDebug() << "Unknown error:" << QString::number(currAnsw.command, 16);
        break;
    }
}

void MainWindow::parceAnsw()
{
    if((currComm.counter > 0) && (currComm.numCh > 0) && (currComm.numClass > -1))
        mdl->setMdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 19, "");
    switch (currComm.command) {
    case 0x15:
        if(currAnsw.nParam > 1)
            parce0x15();
        break;
    case 0x14:
        parce0x14();
        break;
    case 0x01:
        parce0x01();
        break;
    case 0x0c:
        parce0x0c();
        break;
    case 0x6c:
        parce0x6c();
        break;
    case 0x61:
        parce0x61();
        break;
    case 0x69:
        parce0x69();
        break;
    case 0x0e:
        parce0x0e();
        break;
    case 0x03:
        parce0x03();
        break;
    case 0x0a:
        parce0x0a();
        break;
    case 0x12:
        parce0x12();
        break;
    case 0x13:
        parce0x13();
        break;
    case 0x0d:
        parce0x0d();
        break;
    case 0x29:
        parce0x29();
        break;
    case 0x1a:
        parce0x1a();
        break;
    case 0x1b:
        parce0x1b();
        break;
    case 0x1c:
        parce0x1c();
        break;
    case 0x0f:
        parce0x0f();
        break;
    default:
        parceSetCommands();
        break;
    }
}

void MainWindow::help()
{
    QString name = QString("help.chm").arg(QCoreApplication::applicationDirPath());
    if(!QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(name))))
        QMessageBox::critical(this, "Error", "Файл справки не найден", QMessageBox::Yes);
}

void MainWindow::createDev()
{
    numVisibleModelColumns = 25;
    numModelRows = 124;
    mdl = new MyModel(0, 54);
    proxyMdl = new QSortFilterProxyModel;
    proxyMdl->setSourceModel(mdl);
    proxyMdl->setDynamicSortFilter(true);
    tbl = new My_table(proxyMdl, true, this);
    tbl->setSortingEnabled(true);
    tbl->sortByColumn(0, Qt::AscendingOrder);
    tbl->horizontalHeader()->setSortIndicatorShown(false);
    QList<int> lst;
    lst.insert(0, ui->grpBxCommon->width());
    lst.insert(1, ui->monitorTab->width() - lst.at(0) + 1000);
    ui->splitter->setSizes(lst);
    /*for(int i = 0; i < numNoEditableColumns; i++)
        tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);*/
    for(int w = 0; w <= numVisibleModelColumns; w++)
        tbl->setColumnWidth(w, 50);
    tbl->setColumnWidth(0, 52);
    tbl->setColumnWidth(1, 57);
    tbl->setColumnWidth(2, 32);
    tbl->setColumnWidth(3, 57);
    tbl->setColumnWidth(4, 62);
    tbl->setColumnWidth(5, 65);
    tbl->setColumnWidth(6, 32);
    tbl->setColumnWidth(7, 74);
    tbl->setColumnWidth(8, 32);
    tbl->setColumnWidth(9, 32);
    tbl->setColumnWidth(10, 32);
    tbl->setColumnWidth(11, 60);
    tbl->setColumnWidth(12, 72);
    tbl->setColumnWidth(13, 72);
    tbl->setColumnWidth(16, 63);
    tbl->setColumnWidth(17, 63);
    tbl->setColumnWidth(19, 70);
    tbl->setColumnWidth(21, 64);
    tbl->setColumnWidth(22, 62);
    tbl->setColumnWidth(23, 62);
    tbl->setColumnWidth(24, 64);
    tbl->setColumnWidth(25, 85);
    dlgtRwCmbmBx = new DelegateRowCmbBx(tbl);
    for(int i = 6; i < 20; i++)
        tbl->setItemDelegateForColumn(i, dlgtRwCmbmBx);
    connect(mdl, SIGNAL(cellDataChanged(QModelIndex)), this, SLOT(slotReShowCol(QModelIndex)));
    connect(tbl, SIGNAL(drawGraph(QModelIndex)), this, SLOT(drawGraphSlot(QModelIndex)));
    connect(dlgtRwCmbmBx, SIGNAL(signalCellDataChanged(int, int, QString, QString)),
            this, SLOT(itemModelDataChanged(int, int, QString, QString)));
    tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayoutTable->addWidget(tbl);
    initAct();
}

void MainWindow::slotReboot()
{
    Command comm(-1, -1, 0x6f);
    commLst.append(comm);
    answerFlag = true;
    writeLog("Рестарт БРШС", -1, -1, true);
}

void MainWindow::slot6fBootFirmware()
{
    Command comm(-1, -1, 0x6f);
    sendPack(comm);
    answerFlag = true;
    bootFlag = true;
    disconnect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(onPackReceiver(QByteArray)));
    connect(this, SIGNAL(signPackReceive(QByteArray)), bootWGT, SIGNAL(signal_serial_data(QByteArray)));
}

void MainWindow::create0x6d(int nn, int liter, int bindPer, int skipContr)
{
    unsigned char param[3];
    param[0] = nn;
    param[1] = liter;
    param[2] = bindPer + (skipContr << 4);
    Command comm(-1, -1, 0x6d, param, 3);
    commLst.push_front(comm);
}

void MainWindow::printDevices(QStringList l)
{
    m_sde->setDeviceName(currPair.port);
    if(!m_sde->isExists()) {
        lblStatus->setText("Disconnected");
        lblArrow->setPixmap(*out_y);
        //pserial->close();
    }
    else {
        qDebug() << "printDevices";
        tryConnect(pserial, currPair.port, currPair.baud);
        if(lblStatus->text() == "Disconnected") {
            lblStatus->setText("Connected");
            answerFlag = true;
            pb->wait(3000);
        }
    }
}

void MainWindow::slotDelete()
{
    unsigned char numChPar = addrNum();
    Command comm(clsNum(),
            numChPar,
            0x0E,
            &numChPar,
            1);
    commLst.push_back(comm);
}

void MainWindow::slotBind()
{
    for(int i = 0; i < 5; i++) {
        arrFreeChans[i].clear();
        for(int j = 1; j < numModelRows; j++) {
            if(arrOccupiedChans[i].isEmpty() && arrOccupiedChans[i].contains(j))
                break;
            arrFreeChans[i].append(j);
        }
    }
    dialBind = new DialogBinding(bindPeriodInd, skipNum, controlPeriod, arrFreeChans, arrDevNames, slTC);
    connect(dialBind, SIGNAL(signalBind(bool)), this, SLOT(slotBindSS(bool)));
    dialBind->show();
}

void MainWindow::slotBindSS(bool flag)
{
    Command comm;
    if(flag) {
        bindStopFlag = false;
        if(currComm.command == 0x03)
            comm.setCommand(currComm);
        else {
            countBinds = dialBind->getCountBinds();
            if(dialBind->parsStr()) {
                unsigned char par[3];
                addrBindChannel = dialBind->getAdrress();
                par[0] = addrBindChannel;
                par[2] = dialBind->getTypeDev();
                if(par[2])
                    par[1] = (dialBind->getNumCls() << 4) + dialBind->getVer();
                else
                    par[1] = 0;
                comm.setCommand(dialBind->getNumCls(),
                        par[0],
                        0x03,
                        par,
                        3);
                connect(this, SIGNAL(bindTimeout()), dialBind, SLOT(slotTimeout()));
            }
        }
        commLst.push_back(comm);
    }
    else {
        bindStopFlag = true;
        disconnect(this, SIGNAL(bindTimeout()), dialBind, SLOT(slotTimeout()));
        answerFlag = true;
        Command comm(-1, -1, 0x69);
        commLst.push_front(comm);
    }
}

/*int MainWindow::firstDevAddrFind(int numCls)
{
    for(int i = 1; i <= numModelRows; i++) {
        if(!arrOccupiedChans[numCls].contains(i))
            return i;
    }
    if(arrOccupiedChans[numCls].size() > 1)
        return 0;
    else
        return 1;
}*/

int MainWindow::clsNum()
{
    int numRow = tbl->ind.row();
    return proxyMdl->data(proxyMdl->index(numRow, 35), Qt::DisplayRole).toInt();
}

int MainWindow::addrNum()
{
    int numRow = tbl->ind.row();
    return proxyMdl->data(proxyMdl->index(numRow, 31), Qt::DisplayRole).toInt();
}

void MainWindow::askTD1()
{
    clsNum();
}

void MainWindow::askSM()
{
    clsNum();
}

void MainWindow::loadDevSpecMap()
{
    QFile *f = new QFile("codEP.txt");
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
    QChar chCl;
    int devCls, startSmb;
    for(int i = 0; i < lstPairs.size(); i++) {
        startSmb = lstPairs.at(i).indexOf("; 0x");
        if(startSmb > 1) {
            chCl = lstPairs.at(i).mid(startSmb, 8).at(4);
            if(chCl.isDigit()) {
                devCls = chCl.digitValue();
                if(devCls >= 0 && devCls <= 4)
                    arrDevNames[devCls].append(lstPairs.at(i));
            }
            bool ok;
            devSpecificationMap.insert(lstPairs.at(i).mid(startSmb + 4, 4).toInt(&ok, 16),
                                       lstPairs.at(i).left(startSmb));
        }
        else
            qDebug() << "Error. No specification in srting"
                     << i + 1 << ". (In device specification file)/" << startSmb;
        //arrDevNames[devCls].append(lstPairs.at(i));
    }
    lgWgt = new logWidget(arrDevNames);
}

void MainWindow::slotActUpdate()
{
    refreshFlag = true;
    chckBxInterView->setChecked(false);
}

void MainWindow::refresh()
{
    bootFlag = false;
    writeInLogFlag = false;
    answerFlag = true;
    customCommFlag = false;
    for(int i = 0; i < 5; i++) {
        arrDevNames[i].clear();
        arrFreeChans[i].clear();
        arrOccupiedChans[i].clear();
        while(mdl->rowCount())
            mdl->removeRows(0, 1);
    }
    //ui->verticalLayoutTable->removeWidget(tbl);
    chckBxInterView->setChecked(true);
    bindStopFlag = false;
    refreshFlag = false;
    createStartCommands();
}

void MainWindow::slotActLoad()
{
    chckBxInterView->setChecked(false);
    bootWGT->open_dialog();
    disconnect(this, SIGNAL(signPackReceive(QByteArray)), bootWGT, SIGNAL(signal_serial_data(QByteArray)));
    connect(this, SIGNAL(signPackReceive(QByteArray)), this, SLOT(onPackReceiver(QByteArray)));
    refresh();
}

void MainWindow::on_signalSendData(QByteArray a)
{
    answ_line.clear();
    //if(!snifferMode)
        pserial->write(a);
    //pb->wait(80);
    //onProcSerialReceive();

}

void MainWindow::slotInterview(bool flag)
{
    if(flag)
        mainCycle();
}

/*QModelIndex MainWindow::findSourceIndex(QModelIndex _ind)
{
    int naPrxMDL = proxyMdl->data(proxyMdl->index(_ind.row(), 0)).toInt();
    for(int i = 0; i < mdl->rowCount(); i++) {
        if(mdl->mdlDt(i, 0).toInt() == naPrxMDL)
            return mdl->index(i,_ind.column());
    }
}*/

void MainWindow::drawGraphSlot(QModelIndex ind)
{
    int netAddr = proxyMdl->data(proxyMdl->index(ind.row(), 0)).toInt();
    if((graph[netAddr].size() > 0) && (ind.column() == 2)) {
        graphView = new Form(graph[netAddr]);
        QPoint pos = graphView->mapFromGlobal(QCursor::pos());
        graphView->move(pos.x() - 5, pos.y() - 65);
        graphView->show();
        connect(graphView, SIGNAL(closeGraph()), SLOT(closeSignalGraph()));
    }
}

void MainWindow::closeSignalGraph()
{
    disconnect(graphView, SIGNAL(closeGraph()), this, SLOT(closeSignalGraph()));
    delete graphView;
}

void MainWindow::moveEvent(QMoveEvent *ev)
{
    window = ev->pos();
}

void MainWindow::writeLog(QString strMess, int ch, int cl, bool addInTxtEdit)
{
    if((!writeInLogFlag) && (currComm.command == 0x01))
        return;
    //logFile = fopen("log/tableLog.txt", "a+");
    int level = -1;
    if((ch > 0) && (cl > 0) && (currComm.command != 0x03))
        level = mdl->mdlDt(mdl->rowMdl(ch, cl), 2).toInt();
    textCounterLog++;
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    QString str = date.toString("dd.MM.yy");
    str.append("   ");
    str.append(time.toString("hh:mm:ss") + "\n");
    str.append(&strMess);
    if(ch != -1)
        str.append("; канал №" + QString::number(ch));
    if(cl != -1)
        str.append("; класс №" + QString::number(cl));
    if((level > 0) && (commLst.last().command != 0x69)) {
        if(!strMess.isEmpty())
            str.append("; ");
        str.append("\nуровень сигнала: " + QString::number(level));
    }
    str.append("\n");
    //fwrite(str.toLocal8Bit(), str.size(), 1, logFile);
    //fclose(logFile);
    QString db_line;
    //db_line.append(QString::number(row)+", ");
    if((ch > 0) && (cl > 0) && (mdl->rowMdl(ch, cl) != -1) && (currComm.command != 0x03)) {
        db_line.append("'" + mdl->mdlDt(mdl->rowMdl(ch, cl), 27).toString() + "', ");
        db_line.append(mdl->mdlDt(mdl->rowMdl(ch, cl), 0).toString() + ", ");
    }
    db_line.append(QString::number(ch) + ", ");
    db_line.append(QString::number(cl) + ", ");
    db_line.append("'" + strMess + "', ");
    db_line.append("'" + date.toString("yyyy-MM-dd") + " " + time.toString("hh:mm:ss") + "', ");
    if((level > 0) && (commLst.last().command != 0x69))
        db_line.append(QString::number(level));
    else
        db_line.append("NULL");
    //write_t = QTime::currentTime();
    //ui->textEdit_msg->append(write_t.toString("hh:mm:ss.zzz"));
    //QSqlError err = log->db.lastError();
    //write_t = QTime::currentTime();
    //ui->textEdit_msg->append("------>" + write_t.toString("hh:mm:ss.zzz"));
    //delete log;
    /*}
    else*/
    lgWgt->log->insertLine(db_line);
    if(addInTxtEdit) {
        if((db_line.contains("Помеха") && !ui->chckBxNoize->isChecked()) ||
                db_line.contains(SignalErr7911) ||
                db_line.contains(SignalErr7912) ||
                db_line.contains(SignalErr7913) ||
                db_line.contains(SignalErr7914) ||
                db_line.contains(SignalErr7915) ||
                db_line.contains(SignalErr7916) ||
                db_line.contains(SignalErr7917) ||
                db_line.contains(SignalErr7918) ||
                db_line.contains(SignalErr7919))
            return;
        if(textCounterLog > 1000) {
            textCounterLog = 0;
            ui->txtEditSimpleLog->clear();
        }
        ui->txtEditSimpleLog->append(str);
    }
}

void MainWindow::clearTxtEdit(bool)
{
    ui->txtEditSimpleLog->clear();
}

void MainWindow::slotTabChanged(int numTab)
{
    if(numTab == 2)
        slotAskTree();
}

/*void MainWindow::compilAnswerBoot(qint64 ba, QByteArray a)
{
    int parSize, start;
    tempBootArr.append(a);
    start = tempBootArr.indexOf(0xFA);
    if(start != tempBootArr.size()) {
        if((start + 1) < tempBootArr.size()) {
            parSize = tempBootArr.at(start + 1);
            bootArr.clear();
            bootArr.append(tempBootArr.right(tempBootArr.size() - start));
            bootArr = bootArr.left(parSize);
            //emit signPackReceiveBoot(bootArr);

            if(bootArr.size() == (parSize)) {
                tempBootArr.clear();
                //if((answ_line.at(1) == 0x06) && ((answ_line.at(1) > 0) || (answ_line.at(1) < 4)))
                emit signPackReceiveBoot(bootArr);
            }
            emit signPackReceiveBoot(tempBootArr);
        }
    }
    //emit signPackReceiveBoot(a);
}*/
