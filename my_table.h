#ifndef MY_TABLE_H
#define MY_TABLE_H

#include <QTableView>
#include <QWidget>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAbstractItemModel>
#include <QTextCodec>
#include "mymodel.h"
#include "form.h"
#include "global_variables.h"
#include <QApplication>

class My_table : public QTableView
{
    Q_OBJECT
public:
    My_table(QAbstractItemModel *_mdl, bool editOneClick, QWidget *WidgetParent);
    QAbstractItemModel *mdl;
    QModelIndex ind;
    QModelIndexList lstSelInd;
    int occupiedSizeMdl();
    bool isOccupiedRow(QModelIndex _ind);
    bool isOccupiedRow(int _r);
    bool isOccupiedCell(QModelIndex _ind);
    bool isOccupiedCell(int _r, int _c);
    void showCol(bool showFlag, int numCol);
    int currColumn;
private:
    QMenu *menu;
    bool hideFlag;
    QWidget *pPW;
    int modelRows;
    int modelColumns;
    void contextMenuEvent(QContextMenuEvent *event);
private slots:
    void click_slot(QModelIndex i);
    void customHeaderMenuRequested(QPoint pos);
    void slotHideColumn();
signals:
    void signChangeNetSettings(int _row);
    void drawGraph(QModelIndex ind);
    void signHideColumn(bool flagTrue, int column);
};

#endif // MY_TABLE_H
