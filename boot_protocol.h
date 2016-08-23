#ifndef BOOT_PROTOCOL_H
#define BOOT_PROTOCOL_H

#include <QObject>
#include <QEventLoop>
#include <QTimer>

class boot_protocol : public QEventLoop
{
    Q_OBJECT
public:
    explicit boot_protocol(QByteArray *firmware, QObject *parent = 0);
    int reset_device();
    int write_init(int timeout_answ, int offset, int num);
    int writeData(int timeout_answ, int offset);
    int readID(int timeout_answ, QByteArray &answ, unsigned char comm);
    int reset_boot(int timeout_answ);
    QByteArray boot_data;
private:
    QTimer timer;
    int timeout;
    QByteArray *data_firmware;
    int timer_cnt;
    bool reply;
    bool global_cancel;
    QByteArray buff;
    void WritePkt(QByteArray in, int count);
    uint16_t Crc16Reversed(uint16_t poly, uint16_t initvalue, uint16_t afterxor,
                           unsigned char buffer[], int offset, int count);
    int boot_counter;
    QByteArray boot_temp;
    QByteArray boot_answer;
private slots:
    void Read_reply();
    void Read_boot_reply();
    void boot_data_receiver(QByteArray);
    void cancel_firmware();
signals:
    void data(QByteArray);
    void dataString(QString);
    void signal_SendPacket(QByteArray);
    void progressBarSer(int);
    void signalComm6F();
public slots:

};

#endif // BOOT_PROTOCOL_H
