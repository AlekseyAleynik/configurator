#include "log.h"
#include <QMessageBox>
#include <QStandardItemModel>

Log::Log(QString str, QObject *parent) : QObject(parent)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    currentBase = str;
    open_database();
}

Log::~Log()
{
    db.removeDatabase(db.connectionName());
    db.close();
}

void Log::open_database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");    
    db.setDatabaseName(currentBase);
    if(!db.open()) {
        QSqlError err = db.lastError();               
        QMessageBox::critical(0, "Error", err.text(), QMessageBox::Yes);
        return;
    }
    QSqlQuery query(db);
    db.transaction();
    query.exec("create table log ("
                   "name varcharart(50), "
                   "netAddr int, "
                   "channel int, "
                   "class int, "
                   "signal varcharart(50), "
                   "time DATETIME, "
                   "level int, "
                   "PRIMARY KEY(netAddr, channel, signal, time)"
                   ");");
    //b = query.exec("insert into log values(0, 0, 0, 'test', '2013-03-15 21:21:23')");
    //b = query.exec("DROP table log;");
    db.commit();
    query.clear();
    //QSqlError e = query.lastError();
}

bool Log::insertLine(QString s)
{
    //db.open();
    QSqlQuery query(db);
    QString command = "insert into log values(" + s + ")";
    db.transaction();
    query.exec(command);
    db.commit();
    query.lastError();
    db.lastError();
    query.clear();
    return true;
}

QString Log::clearTable()
{
    QSqlQuery query(db);
    db.transaction();
    query.exec("DELETE FROM log");
    db.commit();
    QString err;
    if(query.lastError().isValid()) {
        err = query.lastError().databaseText();
        return err;
    }
    query.clear();
    return "";
}

void Log::showTable(QTableView *tbl, QString col, QString params)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    db.transaction();
    if(!params.isEmpty())
        model->setQuery("SELECT " + col + " FROM log " + "WHERE " + params + " ORDER BY time;");
    else
        model->setQuery("SELECT " + col + " FROM log ORDER BY time;");
    QSqlError err;
    if(model->lastError().isValid())
        err = model->lastError();
    model->setHeaderData(0, Qt::Horizontal, "Имя", Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, "Сетевой\nадрес", Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, "Канал", Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, "Класс", Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, "Сигнал", Qt::DisplayRole);
    model->setHeaderData(5, Qt::Horizontal, "Время", Qt::DisplayRole);
    model->setHeaderData(6, Qt::Horizontal, "Уровень\nсигнала", Qt::DisplayRole);
    tbl->setModel(model);
}

/*void log_db::initializeModel(QSqlTableModel *model, QString strName)
{
    model->setTable(strName);
    model->select();
}

QString log_db::GetCurrentBase()
{
    return currentBase;
}*/
