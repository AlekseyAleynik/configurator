#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QTextCodec>
#include <QPixmap>
#include <vector2droles.h>
#include "global_variables.h"

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit  MyModel(int _rows, int _cols, QObject *pobj = 0);
    MyModel(const MyModel &myMdl);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant data1(const int _row, const int _col, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QByteArray if_multiChannel(int i);
    void setMdlDt(int row, int col, QVariant val, int role = Qt::DisplayRole);
    QVariant mdlDt(int row, int col, int role = Qt::DisplayRole);
    void setMdlDt(QModelIndex ind, QVariant val, int role = Qt::DisplayRole);
    QVariant mdlDt(QModelIndex ind, int role = Qt::DisplayRole);
    int rowMdl(int addr, int numCls);
private:
    inline void clrEl(int i, int j);
    Vector2dRoles *vec;
    int numModelColumns;
    //QString dataCellString[rows][cols];
signals:
    void editCompleted(const QString &);
    void cellDataChanged(QModelIndex index);
};

#endif
