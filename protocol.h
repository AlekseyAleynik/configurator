#ifndef PROTOCOL_H
#define PROTOCOL_H
//#include <qbytearray.h>
#include <QObject>

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = 0);
    ~Protocol();
    virtual QByteArray MakeRequest(unsigned char bltype,
                                   unsigned char bladdr,
                                   unsigned char simb,
                                   unsigned char *pcommand,
                                   unsigned char *pparam,
                                   unsigned char paramlength);
    virtual void setBlocktype(char btype);
    virtual char getBlocktype();
    virtual void setBlockadrr(char addr);
    virtual char getBlockadrr();
    virtual unsigned short Crc(unsigned short poly, unsigned short initvalue,
            unsigned short afterxor, unsigned char *pbuf, int offset, int count);
    virtual bool isStartSequenceClientPack(unsigned char *pack_byte);
    virtual bool isStartSequenceServerPack(unsigned char *pack_byte);
//    virtual unsigned char* EscapeFiltr(unsigned char *pbuf, unsigned char *pdata, int length);
    virtual QByteArray EscapeFiltr(unsigned char *pdata, int length);
//    virtual int return_full_Package_size();
//virtual  void getRespons(char *respons);
/*  virtual    unsigned short  Crc(unsigned short poly, unsigned short initvalue,
                unsigned short afterxor, unsigned char *pbuf, int offset, int count);*/

//  char    _blocktype;
//  char    _blockaddr;
//  char    _specsimb;
//  char    _dlength;
//  unsigned char   _buffer[64];
//  unsigned char _lastRq;    
    unsigned char bltype;
};

#endif // PROTOCOL_H
