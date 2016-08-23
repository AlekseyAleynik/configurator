#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QtSql>
#include <QTableView>
#include <QDateTimeEdit>
#include <QSortFilterProxyModel>
#include <QTextCodec>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QString str = "", QObject *parent = 0);
    ~Log();
    void showTable(QTableView *tbl, QString col, QString params = "");
    bool insertLine(QString s);
    QString clearTable();
    //QString GetCurrentBase();
private:
    void open_database();
    //void initializeModel(QSqlTableModel *model, QString strName);
    QSqlDatabase db;
    QString currentBase;
signals:
    void databaseError(QString);
public slots:
};

#endif // LOG_H
