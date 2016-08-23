#include "boot_protocol.h"
#include <QTime>
#include <QDebug>

boot_protocol::boot_protocol(QByteArray *firmware, QObject *parent) :
    QEventLoop(parent),
    data_firmware(firmware)
{
    timer_cnt = 0;
    boot_counter = 0;
    global_cancel = false;
}

void boot_protocol::WritePkt(QByteArray in, int count)
{
    QByteArray a;
    a.append(0xFA);
    a.append(char(count + 2));
    for(int i = 0; i < count; i++)
        a.append(in[i]);
    unsigned char *buffer = new unsigned char[count + 2];
    for(int i = 0; i < count + 2; i++)
        buffer[i] = a[i];
    uint16_t crc = Crc16Reversed(0x8408, 0xFFFF, 0, buffer, 1, count + 1);
    a.append(char(crc));
    a.append(char(crc >> 8));
    emit this->signal_SendPacket(a);
    delete buffer;
}

int boot_protocol::reset_device()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), SLOT(Read_reply()));
    emit signalComm6F();
    timer.start(1);
    exec();
    timer.stop();
    return 0;
}

int boot_protocol::reset_boot(int timeout_answ)
{
    int ex;
    boot_answer.clear();
    timeout = timeout_answ;
    QByteArray command;
    command.append(0x01);
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), SLOT(Read_boot_reply()));
    for(int i = 0; i < 3; i++) {
        WritePkt(command, 1);
        timer.start(1);
        ex = exec();
        timer.stop();
        if(ex == 0 || ex == 3)
            break;
    }
    return ex;
}

int boot_protocol::readID(int timeout_answ, QByteArray &answ, unsigned char comm)
{
    boot_answer.clear();
    timeout = timeout_answ;
    QByteArray command;
    command.append(comm);
    int ex = -1;
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), SLOT(Read_boot_reply()));
    for(int i = 0; i < 120000; i++) {
        WritePkt(command, 1);
        timer.start(1000);
        ex = exec();
        timer.stop();
        if(ex == 0 || ex == 3)
            break;
    }
    answ.clear();
    answ.append(boot_answer.mid(2, boot_answer.size() - 2));
    return ex;
}

int boot_protocol::write_init(int timeout_answ, int offset, int num)
{
    int ex;
    boot_answer.clear();
    timeout = timeout_answ;
    QByteArray file;
    file.append(0x04);
    //file.append(0x6A);
    //file.append(0x1C);
    file.append(data_firmware->mid(offset, num));
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), SLOT(Read_boot_reply()));
    for(int i = 0; i < 3; i++) {
        WritePkt(file, num + 1);
        timer.start(1);
        ex = exec();
        timer.stop();
        if(ex == 0)
            break;
        if(ex == 2)
            emit dataString("GLOBAL ERROR");
    }
    return ex;
}

int boot_protocol::writeData(int timeout_answ, int offset)
{
    int ex;
    int num = 128;
    int packet = 0;
    bool end = false;
    bool repeat = true;
    boot_answer.clear();
    timeout = timeout_answ;
    QByteArray file;
    file.append(0x05);
    file.append((char)packet);
    file.append(data_firmware->mid(offset, num));
    int progressCnt;
    bool progress = false;
    int b1 = 0;
    if(100 /(data_firmware->size() / num) < 1) {
        for(int i = 2; i < data_firmware->size(); i++) {
            //float val = 100*i/(data_firmware->size()/num);
            if((100 * i / (data_firmware->size() / num)) * i >= 1) {
                progressCnt = i;
                break;
            }

        }
    }
    else {
        progressCnt = 100 / (data_firmware->size() / num);
        progress = true;
    }
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), SLOT(Read_boot_reply()));
    for(int b = 0; b < (data_firmware->size() / num) + 3; b++) {
        packet++;
        if(packet > 255)
            packet = 0;
        if(progress)
            emit progressBarSer(progressCnt);
        else {
            if(b1 > progressCnt) {
                emit progressBarSer(1);
                b1 = 0;
            }
        }
        b1++;
        for(int i = 0; i < 6; i++) {
            emit dataString("writing byte...." + QString::number(b));
            emit dataString("elapsed" + QString::number(data_firmware->mid(offset + num * (b + 1)).size()));
            WritePkt(file, file.size());
            timer.start(1);
            ex = exec();
            timer.stop();
            boot_answer.clear();
            if(ex == 0) {
                repeat = false;
                break;
            }
            if(ex == 2)
                emit dataString("GLOBAL ERROR");
            if(ex == 1)
                emit dataString("TIMEOUT");
            if(end && ex == 5)
                break;
        }
        if(end && ex == 5)
            break;
        if(repeat) {
            repeat = true;
            b--;
            packet--;
        }
        if(ex == 1)
            break;
        file.clear();
        file.append(0x05);
        file.append((char)packet);
        file.append(data_firmware->mid(offset + num * (b + 1), num));
        if(data_firmware->mid(offset + num * (b + 1)).size() < 128) {
            end = true;
            emit dataString("last state");
        }
    }
    return ex;
}

void boot_protocol::boot_data_receiver(QByteArray a)
{
//    emit dataString("---------clearansw--------");
//    emit data(a);
//    emit dataString("---------------------------");
    int start, blockSize;
    bool valid = false;
    bool valid_blockSize = false;
    if(boot_counter == 0) {
        for(int i = 0; i < a.size(); i++) {
            if(a[i] == 0xFA) {
                start = i;
                valid = true;
                break;
            }
        }
    }
    if(boot_counter == 0 && valid) {
        for(int i = start; i < a.size(); i++) {
            boot_temp.append(a[i]);
            boot_counter++;
        }
    }
    if(boot_counter > 0 && !valid) {
        for(int i = 0; i < a.size(); i++) {
            boot_temp.append(a[i]);
            boot_counter++;
        }
    }
    if(boot_counter > 1) {
        blockSize = boot_temp[1];
        valid_blockSize = true;
    }
    if(boot_counter > blockSize && valid_blockSize) {
        for(int i = 0; i < blockSize; i++)
            boot_answer.append(boot_temp[i]);
        boot_counter = 0;
        boot_temp.clear();
    }
}

void boot_protocol::Read_reply()
{
    if(buff[0] == 0xC2) {
        timer_cnt = 0;
        QEventLoop::exit(0);
    }
    if(timer_cnt > 1000) {
        QEventLoop::exit(1);
        timer_cnt = 0;
    }
    timer_cnt++;
}

void boot_protocol::Read_boot_reply()
{
    if(global_cancel) {
        QEventLoop::exit(1);
        timer_cnt = 0;
        return;
    }
    if(boot_answer.size() > 2 && boot_answer[2] == 0) {
        emit data(boot_answer);
        timer_cnt = 0;
        QEventLoop::exit(0);
    }
    else if(boot_answer.size() > 2 && boot_answer[2] == 1) {
        emit data(boot_answer);
        timer_cnt = 0;
        QEventLoop::exit(2);
    }
    else if(boot_answer.size() > 2 && boot_answer[2] == 2) {
        emit data(boot_answer);
        timer_cnt = 0;
        QEventLoop::exit(5);
    }
    else if(boot_answer.size() > 2) {
        emit data(boot_answer);
        timer_cnt = 0;
        QEventLoop::exit(3);
    }
    if(timer_cnt > timeout) {
        QEventLoop::exit(1);
        timer_cnt = 0;
    }
    timer_cnt++;
}
//Crc16Reversed(0x8408, 0xFFFF, 0, buffer, 1, count+1);

uint16_t boot_protocol::Crc16Reversed(uint16_t poly, uint16_t initvalue, uint16_t afterxor,
                                      unsigned char buffer[], int offset, int count)
{
    uint16_t crc = initvalue;
    for (int i = offset; i < (offset + count); i++) {
        crc ^= buffer[i];
        for (int j = 0; j < 8; j++) {
            if ((crc & 0x0001) != 0)
                crc = (uint16_t)((crc >> 1) ^ poly);
            else
                crc = (uint16_t)(crc >> 1);
        }
    }
    return (uint16_t)(crc ^ afterxor);
}

void boot_protocol::cancel_firmware()
{
    global_cancel = true;
}
