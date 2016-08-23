#ifndef PROTOCOL485RK_H
#define PROTOCOL485RK_H

#include <QObject>
#include "protocol.h"
#include <QDebug>

class Protocol485RK : public Protocol
{
    Q_OBJECT
public:
    explicit Protocol485RK(QObject *parent = 0);
    ~Protocol485RK();
    QByteArray MakeRequest(//unsigned char *pbuffer,
                               unsigned char bltype,
                               unsigned char bladdr,
                               unsigned char simb,
                               unsigned char *pcommand,
                               unsigned char *pparam,
                               unsigned char paramlength);
	void setBlocktype(char btype);
    char getBlocktype();
	void setBlockadrr(char addr);
	char getBlockadrr();
    virtual bool isStartSequenceClientPack(unsigned char *pack_byte);
    virtual bool isStartSequenceServerPack(unsigned char *pack_byte);
	//void getRespons(char *respons);
signals:
public slots:
private:
    unsigned short  Crc(unsigned short poly, unsigned short initvalue,
                        unsigned short afterxor, unsigned char *pbuf, int offset, int count);
//    unsigned char* EscapeFiltr(unsigned char *pbuf, unsigned char *pdata, int length);
    QByteArray EscapeFiltr(unsigned char *pdata, int length);
    char _blocktype;
    char _blockaddr;
    char _specsimb;
    char _dlength;
    unsigned char _buffer[64];
};

#endif // PROTOCOL485RK_H
