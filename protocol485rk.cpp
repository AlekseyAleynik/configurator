#include "protocol485rk.h"

Protocol485RK::Protocol485RK(QObject *parent) : Protocol(parent)
{
    //memset(buffer, 0, sizeof(buffer));
}

Protocol485RK::~Protocol485RK()
{}

void Protocol485RK::setBlocktype(char btype)
{
    _blocktype = btype;
}
char Protocol485RK::getBlocktype()
{
    return _blocktype;
}
void Protocol485RK::setBlockadrr(char addr)
{
    _blockaddr = addr;
}
char Protocol485RK::getBlockadrr()
{
    return _blockaddr;
}

unsigned short Protocol485RK::Crc(unsigned short poly, unsigned short initvalue,
            unsigned short afterxor, unsigned char *pbuf, int offset, int count)
{
    //CCITT 0x8408 reversed, initvalue-0xFFFF, finalxor-none
           //-----begin-----
           //By Nic Poniatovskiy
   unsigned short crc = initvalue;
   pbuf += offset;
   for (int i = offset; i < (offset + count); i++)
   {
       crc ^= *pbuf;
       ++pbuf;
       for (int j = 0; j < 8; j++)
       {
           if ((crc & 0x0001) != 0)
               crc = (unsigned short)((crc >> 1) ^ poly);
           else
               crc = (unsigned short)(crc >> 1);
       }
   }
   return (unsigned short)(crc ^ afterxor);
}
//Escape С2 С1 C0 everywhere expect for the header
//Return new length
//unsigned char* Protocol485RK::EscapeFiltr(unsigned char *pbuf, unsigned char *pdata, int length)
QByteArray Protocol485RK::EscapeFiltr(unsigned char *pdata, int length)
{
    QByteArray ba(reinterpret_cast<const char*>(pdata), length);
    for(int i = 1; i < length; i++)
    {
        switch(ba.at(i))
        {
            case (char)0xc0:
                i++;
                ba.insert(i, (char)0x00);
                break;
            case (char)0xc1:
                ba[i] = (char)0xc0;
                i++;
                ba.insert(i, (char)0x01);
                break;
            case (char)0xc2:
                ba[i] = (char)0xc0;
                i++;
                ba.insert(i, (char)0x02);
                break;
            default:
                break;
        }
    }
    return ba;
}

QByteArray Protocol485RK::MakeRequest(//unsigned char *pbuffer,
                           unsigned char bltype,
                           unsigned char bladdr,
                           unsigned char simb,
                           unsigned char *pcommand,
                           unsigned char *pparam,
                           unsigned char paramlength)
{
    unsigned char buf[64];
    memset(buf, 0, sizeof(buf));   //optional
    unsigned int crc;
    int i = 0;
    buf[i] = (unsigned char)0xC1;
    i++;
    buf[i] = bltype;
    i++; //Device type (6)
    buf[i] = bladdr;
    i++; // Address by line RS485
    buf[i] = simb;
    i++;  //Service byte
    buf[i] = (char)paramlength + 1;
    i++; // length = command + parametr
    //Данные
    buf[i] = *pcommand;
    i++;
    for (int k = 0; k < paramlength; ++k)
    {
        buf[i] = *pparam;
        ++i;
        ++pparam;
    }
    crc = Crc(0x8408, 0xFFFF, 0, &buf[0], 0, i);
    buf[i] = (unsigned char)(crc & 0xFF);
    i++;   // CRCLo
    buf[i] = (unsigned char)((crc >> 8) & 0xFF);
    i++; // CRCHi
//    return QByteArray(reinterpret_cast<const char*>(EscapeFiltr(pbuffer, &buf[0], i)), i);
    QByteArray x = EscapeFiltr(buf, i);
    return x;
    //return EscapeFiltr(buf, i);
}

bool Protocol485RK::isStartSequenceClientPack(unsigned char *pack_byte)
{
    return (bool)(pack_byte[0] == 0xC2);
}

bool Protocol485RK::isStartSequenceServerPack(unsigned char *pack_byte)
{
    return (bool)(pack_byte[0] == 0xC1);
}
