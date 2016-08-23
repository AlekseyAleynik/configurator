#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QSplitter>
#include <QTextEdit>
#include <QSettings>
#include <QMessageBox>
#include <cmath>
#include <QTableWidgetItem>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QStandardItem>
#include <QCheckBox>
#include "AboutDilog.h"
#include "command.h"
#include "form.h"
#include "log.h"
#include "dialogselectconnection.h"
#include "serialdeviceenumerator.h"
#include "global_variables.h"
#include "bootwidget.h"
#include "protocol.h"
#include "protocol485rk.h"
#include "protocolsigmark.h"
#include "vector2dint.h"
#include "mymodel.h"
#include "setnewnet.h"
#include "PortBaud.h"
#include "dialogbinding.h"
#include "newcomm.h"
#include "delegaterowcmbbx.h"
#include "delegaterowspnbx.h"
#include "mymodel.h"
#include "my_table.h"
#include "logwidget.h"
#include "treemodel.h"
#include "treeitem.h"
//#include "treedev.cpp"

extern int s_cnt_sys_bytes;
extern int s_cnt_start_bytes;
extern int s_num_rep_byte;
extern int s_num_s_com_byte;
extern int s_num_c_com_byte;
extern int numBytes0x15com;
extern int numModelRows;
extern int numVisibleModelColumns;
extern int periodControl;
extern unsigned char global_dev_type;
extern unsigned char broadcast_adr;
extern int currTab;

namespace Ui {
class MainWindow;
}

class AbstractSerial;
class BootWidget;
class Dev_class;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    BootWidget *bootWGT;
    ~MainWindow();
    Command currComm;
    Command currAnsw;
    My_table *tbl;
    QSortFilterProxyModel *proxyMdl;
    MyModel *mdl;
    DelegateRowCmbBx *dlgtRwCmbmBx;
    bool qEx;
    bool writeInLogFlag;
    DialogBinding *dialBind;
    logWidget *lgWgt;
private:
    bool refreshFlag;
    bool customCommFlag;
    int startByte;
    int len_pack, c_b, add;
    QByteArray answ_line, tempByteArr, bootArr, tempBootArr;
    Pair currPair;
    PortBaud *pb;
    QStringList availablePorts;
    bool answerFlag;
    bool bootFlag;
    Protocol485RK *pprotocol485RK;
    //ProtocolSIGMArk *pprotocolSigma;
    //Protocol *pprotocol;
    QStringList baudList;
    AbstractSerial *pserial;
    SerialDeviceEnumerator *m_sde;
    QStringList SerialItems;
    DialogSelectConnection *dsc;
    QSettings _settings;
    Ui::MainWindow *ui;
    QPixmap *out_y;
    QPixmap *out_g;
    QList<Command> commLst;
    QList<NewComm> newCommList;
    QStringList devModelsList;
    Vector2dInt *matrixObj0x15, *matrixObj0x1C;
    int lvl;
    QLabel *lblArrow;
    QLabel *lblSN;
    QLabel *lblPOVer;
    QLabel *lblNetNum;
    QLabel *lblNumLiter;
    QLabel *lblPort;
    QLabel *lblSysAddress;
    int bindPer;
    //QLabel *lblControlPeriod;
    //QLabel *lblSkipNum;
    //QLabel *lblBindPeriod;
    //QLabel *lblBindPeriod_txt;
    //QLabel *lblSkipNum_txt;
    //QLabel *lblControlPeriod_txt;
    QLabel *lblCoverStat;
    QLabel *lblBindStat;
    QLabel *lblIn1;
    QLabel *lblIn2;
    QLabel *lblNoize;
    QLabel *lblStatus;
    QLabel *lblInterView;
    QCheckBox *chckBxInterView;
    int textCounterSwap;
    int textCounterLog;
    int netNum;
    int numLiter;
    int bindPeriodInd;
    int skipNum;
    int sysAddress;
    QString controlPeriod;
    QStringList slTC;
    int numModeTrubachInt;
    int countBinds;
    int addrBindChannel;
    QStringList arrDevNames[5];
    QMap<int, QString> devSpecificationMap;
    QVector<int> arrOccupiedChans[5];
    QVector<int> arrFreeChans[5];
    //FILE *logFile;
    bool bindStopFlag;
    bool startLogFlag;
    Form *graphView;
    QMap<QString, int> graph[124];
    QPoint window;
    QStringList commandsVec[5];
    QVector<QVariant> headersLst;
    //QStandardItemModel *treeMdl;
    TreeModel *treeMdl;
    QList<QStandardItem *> prepareRow(QStringList rowLst);
    void initAct();
    void moveEvent(QMoveEvent *);
    void closeSignalGraph();
    void writeLog(QString strMess, int ch, int cl, bool addInTxtEdit);
    void compilAnswer(qint64 ba, QByteArray a);
    //void compilAnswerBoot(qint64 ba, QByteArray a);
    QString separateStr(QString shortStr, QString mainStr);
    void create0x6d(int nn, int liter, int bindPer, int skipContr);
    void create0x0d(int cls, int row, int bindPer, int skipContr);
    void parceAnsw();
    void parce0x15();
    void parce0x14();
    void parce0x01();
    void parce0x0f();
    void parceParam0x01(Command &_comm);
    void parce0x61();
    void parce0x6c();
    void parce0x69();
    void parce0x0e();
    void parce0x0c();
    void parce0x03();
    void parce0x0a();
    void parce0x12();
    void parce0x13();
    void parce0x0d();
    void parce0x1a();
    void parce0x1b();
    void parce0x1c();
    void parce0x29();
    void parceSetCommands();
    void createDev();
    void cellWork(Command &_comm, int numByte, int bitVal, int col, QString mess1 = "", QString mess2 = "");
    void initConnectParams();
    void startFunctions();
    void closeApp(QString exitText);
    QString translateTrubachMode(int par);
    QString convertBindPer(int indBindPer);
    void loadDevSpecMap();
    QByteArray createParam(QModelIndex &ind, QString strNew, int numCls);
    QByteArray paramCreator(int intPar1);
    unsigned char commByteCreate(int numColumn);
    int translateChannelTwo(int par);
    int parceParamTrubach(QString str);
    int parceParamTrubach_2(QString str);
    void parceErrorAnswer();
    //int firstDevAddrFind(int numCls);
    int clsNum();
    int addrNum();
    void boot(QByteArray fwba);
    //QModelIndex findSourceIndex(QModelIndex _ind);
    void loadCommLst();
    void parceFile(QString fileName, int numCls);
    void sendCustomComm();
    void addSpecCommands(int row);
    void addInTxtEditSwap(QByteArray baPack);
    void translateChannelOne(int row, int col, int par);
    void insertChild(QModelIndex index, QString dev, int channel, int cls);
    void insertRow(QModelIndex index, QString dev, int channel, int cls);
    void removeRow(QModelIndex index);
    void updateActions();
    int findRouter();
private slots:
    void clearTxtEdit(bool);
    void itemModelDataChanged(int row, int col, QString newStr, QString oldStr);
    void on_signalSendData(QByteArray a);
    void slotHelp();
    void slotConnectTo();
    void slotReboot();
    void slot6fBootFirmware();
    void slotAboutApp();
    void slotSetParams();
    void parceAnswerPack(QByteArray ba);
    //void on_procSerialReceive(QByteArray a);
    void help();
    void onPackReceiver(QByteArray pack);
    void onProcSerialReceive();
    void printDevices(QStringList);
    void setBaudSlot(int baud);
    void slotSetSettings();
    void slotCancelSettings();
    void slotChangeCls(int currInd);
    void slotChangeCmnd(QString cmnd);
    void slotPBSend();
    void slotInterview(bool flag);
    void slotReShowCol(QModelIndex _ind);
    void slotShowHideAct_0(bool showHideFlag);
    void slotShowHideAct_1(bool showHideFlag);
    void slotShowHideAct_2(bool showHideFlag);
    void slotShowHideAct_3(bool showHideFlag);
    void slotShowHideAct_4(bool showHideFlag);
    void slotShowHideAct_5(bool showHideFlag);
    void slotShowHideAct_6(bool showHideFlag);
    void slotShowHideAct_7(bool showHideFlag);
    void slotShowHideAct_8(bool showHideFlag);
    void slotShowHideAct_9(bool showHideFlag);
    void slotShowHideAct_10(bool showHideFlag);
    void slotShowHideAct_11(bool showHideFlag);
    void slotShowHideAct_12(bool showHideFlag);
    void slotShowHideAct_13(bool showHideFlag);
    void slotShowHideAct_14(bool showHideFlag);
    void slotShowHideAct_15(bool showHideFlag);
    void slotShowHideAct_16(bool showHideFlag);
    void slotShowHideAct_17(bool showHideFlag);
    void slotShowHideAct_18(bool showHideFlag);
    void slotShowHideAct_19(bool showHideFlag);
    void slotShowHideAct_20(bool showHideFlag);
    void slotShowHideAct_21(bool showHideFlag);
    void slotShowHideAct_22(bool showHideFlag);
    void slotShowHideAct_23(bool showHideFlag);
    void slotShowHideAct_24(bool showHideFlag);
    void slotShowHideAct_25(bool showHideFlag);
    void slotShowHideLogAct(bool showHideFlag);
    void slotHideActCommon(bool hideTrue, int currColumn);
    void slotActLoad();
    void slotTabChanged(int numTab);
    void slotAskTree();
    //void dialogOpenFileSlot();
public slots:
    void slotTimeout0x01();
    void slotDelAll();
    void slotOnForAll();
    void slotOffForAll();
    void slotDelSel();
    void slotOnForSel();
    void slotOffForSel();
    void slotDelete();
    void slotBind();
    void slotBindSS(bool);
    void askTD1();
    void askSM();
    void setAddr(int _addr);
    bool sendTryConn(QString _port, QString _baud);
    bool tryConnect(AbstractSerial *_pser, QString _port, QString _baud);
    void sendPack(Command &_comm);
    void createStartCommands();
    void mainCycle();
    void boot_connect();
    void slotChangeDevNetSettings();
    void slotSortTblView(int col = 0, Qt::SortOrder so = Qt::AscendingOrder);
    void refresh();
    void slotActUpdate();
    void drawGraphSlot(QModelIndex ind);
    void slotDropTreeNode(int routeAddr, int routeCls,
                          int sourceAddr, int sourceCls, unsigned char commByte);
signals:
    void bindTimeout();
    void signalDel();
    void signPackReceive(QByteArray);
protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
