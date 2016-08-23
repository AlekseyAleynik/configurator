#include "my_table.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>

My_table::My_table(QAbstractItemModel *_mdl, bool editOneClick, QWidget *WidgetParent)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    this->setMouseTracking(true);
    mdl = _mdl;
    modelRows = mdl->rowCount();
    modelColumns = mdl->columnCount();
    menu = new QMenu(this);
    hideFlag = false;
    this->setModel(mdl);
    pPW = WidgetParent;
    //this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    this->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customHeaderMenuRequested(QPoint)));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(click_slot(QModelIndex)));
    connect(this->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            pPW, SLOT(slotSortTblView(int, Qt::SortOrder)));
    if(editOneClick)
        connect(this, SIGNAL(pressed(QModelIndex)), SLOT(edit(QModelIndex)));
    //this->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
    QPalette pal;
    QColor buttontext(0, 0, 0);
    pal.setBrush(QPalette::HighlightedText, buttontext);
    this->setPalette(pal);
}

void My_table::contextMenuEvent(QContextMenuEvent *event)
{
    QString del, /*bind,*/ askThreshold, askSettModes, connectSettings;
    QAction *actDel;
    QAction *actBind;
    QAction *actAskThreshold;
    QAction *actAskSettModes;
    QAction *actConnectSettings;
    del = QString("Удалить ОУ");
    //bind = QString("Привязать ОУ");
    askThreshold = QString("Узнать порог для %1 канала").arg(QString::number(ind.row() + 1));
    /*askSettModes = QString("Запросить параметры режимов работы для %1 канала").arg(
            QString::number(ind.row() + 1));*/
    connectSettings = QString("Настроить параметры связи с ОУ");
    if(hideFlag) {
        actDel = new QAction(del, this);
        menu->addAction(actDel);
        connect(actDel, SIGNAL(triggered()), pPW, SLOT(slotDelete()));
        /*if((ind.column() == 17 || ind.column() == 18) ||
                ((ind.column() == 16) && (mdl->data(mdl->index(ind.row(), 17), Qt::DisplayRole).toString() != "") ||
                (mdl->data(mdl->index(ind.row(), 17), Qt::BackgroundColorRole) == QColor(Qt::white)))) {
            actAskThreshold = new QAction(askThreshold, this);
            actAskSettModes = new QAction(askSettModes, this);
            menu->addAction(actAskThreshold);
            connect(actAskThreshold, SIGNAL(triggered()), pPW, SLOT(askTD1()));
            connect(actAskThreshold, SIGNAL(triggered()), pPW, SLOT(askSM()));
        }*/
        if(ind.column() == 5) {
            actConnectSettings = new QAction(connectSettings, this);
            menu->addAction(actConnectSettings);
            connect(actConnectSettings, SIGNAL(triggered()), pPW, SLOT(slotChangeDevNetSettings()));
        }
    }
    /*else {
        actBind = new QAction(bind, this);
        menu->addAction(actBind);
        connect(actBind, SIGNAL(triggered()), pPW, SLOT(slotBind()));
    }*/
    menu->exec(event->globalPos());
    menu->clear();
    hideFlag = false;
}

void My_table::click_slot(QModelIndex i)
{
    ind = i;
    if(isOccupiedCell(i) && !mdl->data(i, Qt::DisplayRole).toString().isEmpty()) {
        if(i.column() == 5)
            emit signChangeNetSettings(i.row());
        hideFlag = true;
    }
    if(i.column() == 2)
        emit drawGraph(i);
}

void My_table::customHeaderMenuRequested(QPoint pos)
{
    currColumn = horizontalHeader()->logicalIndexAt(pos);
    QString headerStr;
    headerStr = mdl->headerData(currColumn, Qt::Horizontal, Qt::DisplayRole).toString();
    headerStr.simplified();
    QMenu *menu = new QMenu(this);
    QAction *actDelAll, *actHideColumn, *actOnForAll, *actOffForAll;
    if(currColumn < 5) {
        actDelAll = new QAction("Удалить все устройства", this);
        menu->addAction(actDelAll);
        connect(actDelAll, SIGNAL(triggered()), pPW, SLOT(slotDelAll()));
    }
    else {
        if(currColumn < 18) {
            actOnForAll = new QAction("Вкл. " + headerStr + " для всех ОУ", this);
            actOffForAll = new QAction("Вылк. " + headerStr + " для всех ОУ", this);
        }
        menu->addAction(actOnForAll);
        menu->addAction(actOffForAll);
        connect(actOnForAll, SIGNAL(triggered()), pPW, SLOT(slotOnForAll()));
        connect(actOffForAll, SIGNAL(triggered()), pPW, SLOT(slotOffForAll()));
    }
    actHideColumn = new QAction("Скрыть колонку", this);
    menu->addAction(actHideColumn);
    connect(actHideColumn, SIGNAL(triggered()), this, SLOT(slotHideColumn()));
    menu->popup(horizontalHeader()->viewport()->mapToGlobal(pos));
    lstSelInd = selectedIndexes();
    if(!lstSelInd.isEmpty()) {
        bool oneColumnFlag = true;
        for(int i = 1; i < lstSelInd.size(); i++) {
            if(lstSelInd.at(i - 1).column() != lstSelInd.at(i).column())
                oneColumnFlag = false;
        }
        if(!oneColumnFlag)
            return;
        QAction *actDelSel, *actOnForSel, *actOffForSel;
        ind = lstSelInd.first();
        if(ind.column() < 5) {
            actDelSel = new QAction("Удалить выделенные устройства", this);
            menu->addAction(actDelSel);
            connect(actDelSel, SIGNAL(triggered()), pPW, SLOT(slotDelSel()));
        }
        else {
            if(ind.column() < 18) {
                actOnForSel = new QAction("Вкл. " + headerStr + " для выделенных ОУ", this);
                actOffForSel = new QAction("Вылк. " + headerStr + " для выделенных ОУ", this);
            }
            menu->addAction(actOnForSel);
            menu->addAction(actOffForSel);
            connect(actOnForSel, SIGNAL(triggered()), pPW, SLOT(slotOnForSel()));
            connect(actOffForSel, SIGNAL(triggered()), pPW, SLOT(slotOffForSel()));
        }
    }
}

int My_table::occupiedSizeMdl()
{
    int count = 0;
    for(int i = 0; i < modelRows; i++) {
        if(mdl->data(mdl->index(0, i), Qt::BackgroundColorRole) == QColor(Qt::gray))
            return count;
        else
            count++;
    }
}

bool My_table::isOccupiedRow(QModelIndex _ind)
{
    if(mdl->data(mdl->index(_ind.row(), 0), Qt::BackgroundColorRole) == QColor(Qt::gray))
        return false;
    else
        return true;
}

bool My_table::isOccupiedRow(int _r)
{
    if(mdl->data(mdl->index(_r, 0), Qt::BackgroundColorRole) == QColor(Qt::gray))
        return false;
    else
        return true;
}

bool My_table::isOccupiedCell(QModelIndex _ind)
{
    if(mdl->data(_ind, Qt::BackgroundColorRole) == QColor(Qt::gray))
        return false;
    else
        return true;
}

bool My_table::isOccupiedCell(int _r, int _c)
{
    if(mdl->data(mdl->index(_r, _c), Qt::BackgroundColorRole) == QColor(Qt::gray))
        return false;
    else
        return true;
}

void My_table::slotHideColumn()
{
    emit signHideColumn(true, currColumn);
    this->hideColumn(currColumn);
}

void My_table::showCol(bool showFlag, int numCol)
{
    if(showFlag)
        this->showColumn(numCol);
    else
        this->hideColumn(numCol);
}
