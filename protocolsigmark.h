#ifndef PROTOCOLSIGMARK_H
#define PROTOCOLSIGMARK_H

#include <QObject>
#include "protocol.h"

/*
Стартовая последовательность    2       Для БЦ: 0B6h 049h  Для СУ: 0B9h 046h
Адрес Приемника                 3       HID СУ 0x06  0x01 0x01
Длина сообщения                 1       Длина информационной части сообщения в байтах (1 – 255)
Информационная часть            1-255   Тело информационного сообщения. Первый байт информационной части сообщения является кодом команды сообщения. Остальные байты являют-ся параметрами этой команды
CRC16 CCITT                     2       Значение CRC, вычисленное для всего пакета

Приемо-передающая аппаратура должна обеспечить исключение стартовой последовательности

typedef struct {
uchar Type; // type of hardware
uint SerialNumber; // 16-bit serial number
} HardwareID;
//Hardware.Type – имеет значение 1 для БЦП
//HardwareID.SerialNumber – серийный номер БЦП

//От СУ
#define OutMarker1   		0xB9
#define OutMarker2		    0x46
//#define DevType			 0x12
//От БЦ:
#define Marker1      		0xB6
#define Marker2      		0x49
*/
//#define shell_package_size 9

class ProtocolSIGMArk : public Protocol
{
    Q_OBJECT
public:
    explicit ProtocolSIGMArk(QObject *parent = 0);
    ~ProtocolSIGMArk();
    //bool CompareAnswer(QByteArray in_buf);
    QByteArray MakeRequest(unsigned char *pbuffer,
                               unsigned char bltype,
                               unsigned char SNL,
                               unsigned char SNH,
                               unsigned char *pcommand,
                               unsigned char *pparam,
                               unsigned char paramlength);
    void setBlocktype(char btype);
    char getBlocktype();
    void setBlockadrr(char addr);
    char getBlockadrr();
    virtual bool isStartSequenceClientPack(unsigned char *pack_byte);
    virtual bool isStartSequenceServerPack(unsigned char *pack_byte);
    unsigned short  Crc(unsigned short poly, unsigned short initvalue,
                        unsigned short afterxor, unsigned char *pbuf, int offset, int cnt);
//   int return_full_Package_size();
signals:
public slots:
private:
   /*unsigned short Crc(unsigned short poly, unsigned short initvalue,
    *                   unsigned short afterxor, unsigned char *pbuf, int offset, int cnt);*/
//    unsigned char* EscapeFiltr(unsigned char *pbuf, unsigned char *pdata, int length);
    QByteArray EscapeFiltr(unsigned char *pdata, int length);
    int full_package_size;
    char _blocktype;
    char _blockaddr;
    char _specsimb;
    char _dlength;
    unsigned char _buffer[64];
};

#endif // PROTOCOLSIGMARK_H
