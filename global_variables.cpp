#include <QStringList>
#include "protocol485rk.h"
#include "protocolsigmark.h"
#include "protocol.h"

// Global variables ------------

//QStringList baud;
//baud << "9600" << "57600" << "115200";
int s_cnt_sys_bytes; // count system bytes in packs
int s_cnt_start_bytes; // count start bytes in packs
int s_num_rep_byte; // number repeate byte
int s_num_s_com_byte; // number byte of length in server pack
int s_num_c_com_byte; // number byte of length in client pack
int numBytes0x15com;
int numModelColumns;
int numVisibleModelColumns;
int numModelRows;
int numNoEditableColumns;
int periodControl;
int currTab;
unsigned char global_dev_type;
unsigned char broadcast_adr;
Protocol485RK *pprotocol485RK;
ProtocolSIGMArk *pprotocolSigma;
Protocol *pprotocol;
