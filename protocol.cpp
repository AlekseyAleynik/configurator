#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{}

Protocol::~Protocol()
{}

void Protocol::setBlocktype(char btype)
{}

char Protocol::getBlocktype()
{}

void Protocol::setBlockadrr(char addr)
{}

char Protocol::getBlockadrr()
{}

unsigned short Protocol::Crc(unsigned short poly, unsigned short initvalue,
                                   unsigned short afterxor, unsigned char *pbuf, int offset, int cnt)
{}

bool Protocol::isStartSequenceClientPack(unsigned char *pack_byte)
{}

bool Protocol::isStartSequenceServerPack(unsigned char *pack_byte)
{}

//Escape C2 C1 C0 everywhere expect for the header
//Return new length
//unsigned char* Protocol::EscapeFiltr(unsigned char *pbuf, unsigned char *pdata, int length)
QByteArray Protocol::EscapeFiltr(unsigned char *pdata, int length)
{}
/*
//Substitute simbols C2 C1 C0.
//Return new length
int Protocol::EscapeRespFiltr(unsigned char *pbuf, unsigned char *pdata, int length)
{}
*/

QByteArray Protocol::MakeRequest(unsigned char bltype,
                                 unsigned char snl,
                                 unsigned char snh,
                                 unsigned char *pcommand,
                                 unsigned char *pparam,
                                 unsigned char paramlength)
{}

/*int Protocol::return_full_Package_size()
{}*/
/*
void Protocol::getRespons(char *respons)
{}
*/
