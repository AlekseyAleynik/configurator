#include "mainwindow.h"

void MainWindow::dialogOpenFileSlot()
{
    QString str = QFileDialog::getOpenFileName(this, "Open Dialog", "", "Firmware (*.crypt);; All (*.*)");
    if(str.size() < 1)
        return NULL;
    ui->open_line->setText(str);
    QFile file_firmware(str);
    if(!file_firmware.exists()) {
        QMessageBox::critical(this, "File error", "File doesn't exist!", QMessageBox::Ok);
        return NULL;
    }
    if(!file_firmware.open(QIODevice::ReadOnly))
        QMessageBox::critical(this, "File error", "File could not be opened!", QMessageBox::Ok);
    boot(file_firmware.readAll());
    file_firmware.close();
    return str;
}

void MainWindow::boot(QByteArray fwba)
{
    if(fwba.size() < 3)
        return;
}
