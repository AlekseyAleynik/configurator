#include <QMessageBox>
#include "bootwidget.h"

BootWidget::BootWidget(MainWindow *main)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    pMainWindow = main;
}

BootWidget::~BootWidget()
{
}

bool BootWidget::setupBoot()
{
    if(data.size() < 3)
        return false;
    pDlg = new QProgressDialog("Запись прошивки", "Отмена", 0, 100, this);
    pDlg->setWindowTitle("Прошивка");
    pDlg->show();
    //pDlg->exec();
    //int i_a_c = 0;
    //int data_size_test = data.size();
    b_prot = new boot_protocol(&data, this);
    connect(b_prot, SIGNAL(data(QByteArray)), SLOT(setupText(QByteArray)));
    connect(b_prot, SIGNAL(dataString(QString)), SLOT(setupMessage(QString)));
    connect(b_prot, SIGNAL(signal_SendPacket(QByteArray)), SIGNAL(signal_SendPacket(QByteArray)));
    connect(b_prot, SIGNAL(progressBarSer(int)), SLOT(setProgressBar(int)));
    connect(pDlg, SIGNAL(canceled()), b_prot, SLOT(cancel_firmware()));
    connect(b_prot, SIGNAL(signalComm6F()), this, SIGNAL(sign6f()));
    QByteArray answ_boot;
    ex = b_prot->reset_device();
    if(ex == 1)
        return false;
    pMainWindow->boot_connect();
    connect(this, SIGNAL(signal_serial_data(QByteArray)), b_prot, SLOT(boot_data_receiver(QByteArray)));
    pDlg->setLabelText("Инициализация");
    ex = b_prot->readID(500, answ_boot, 0x02);
    if(ex == 0 || ex == 3) {
        QString s;
        for(int i = 1; i < answ_boot.size(); i++)
            s.append(answ_boot.at(i));
        ex = b_prot->write_init(1000, 0, 2);
    }
    else {
        if(ex == 1)
            error_occ("Timeout");
        else if(ex == 2)
            error_occ("Error during writing firmware");
        pDlg->accept();
        delete pDlg;
        return false;
    }
    pDlg->setLabelText("Запись прошивки");
    if(!ex)
        ex = b_prot->writeData(3000, 2);
    switch (ex) {
    case 0:
        error_occ("Error writing all firmware");
        return false;
        break;
    case 1:
        pDlg->accept();
        delete pDlg;
        error_occ("timeout");
        return false;
        break;
    case 2:
        pDlg->accept();
        delete pDlg;
        error_occ("Error during writing firmware");
        return false;
        break;
    case 5:
        pDlg->setLabelText("Прошивка записана успешно");
        break;
    default:
        break;
    }
    answ_boot.clear();
    ex = b_prot->readID(1000, answ_boot, 0x03);
    if(ex == 0 || 3) {
        QString s;
        for(int i = 1; i < answ_boot.size(); i++)
            s.append(answ_boot.at(i));
    }
    b_prot->reset_boot(1000);
    pMainWindow->boot_connect();
    delete b_prot;
    QMessageBox::information(this, "Обновление ПО", "Прошивка успешно загружена", QMessageBox::Yes);
    pDlg->accept();
    delete pDlg;
    return true;
}

void BootWidget::setupText(QByteArray b)
{
    QString s;
    foreach (uchar c, b)
        s += "0x" + QString::number(c, 16) + " ";
}

void BootWidget::setupMessage(QString s)
{
    //qDebug() << "s =" << s;
}

QString BootWidget::open_dialog()
{
    QString str = QFileDialog::getOpenFileName(this, "Open Dialog", "", "Firmware (*.crypt);; All (*.*)");
    if(str.size() < 1)
        return NULL;
    QFile file_firmware(str);
    if(!file_firmware.exists()) {
        QMessageBox::critical(this, "File error", "File doesn't exist!", QMessageBox::Ok);
        return NULL;
    }
    if(!file_firmware.open(QIODevice::ReadOnly))
        QMessageBox::critical(this, "File error", "File could not be opened!", QMessageBox::Ok);
    data = file_firmware.readAll();
    file_firmware.close();
    setupBoot();
    return str;
}

void BootWidget::setProgressBar(int i){
    pDlg->setValue(pDlg->value() + i);
}

void BootWidget::error_occ(QString s)
{
    pMainWindow->boot_connect();
    delete b_prot;
    int i = QMessageBox::critical(this, "Error", s + "Try again", QMessageBox::Yes | QMessageBox::No);
    if(i == QMessageBox::Yes)
        setupBoot();
}

/*void BootWidget::slot_data_ret()
{
    //QMessageBox::critical(this, "Прошивка", "Загрузку прошивки невозможно отменить!", QMessageBox::Ok);
}*/

