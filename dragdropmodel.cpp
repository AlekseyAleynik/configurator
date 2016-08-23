#include <QtGui>
#include <QMessageBox>

#include "dragdropmodel.h"

DragDropModel::DragDropModel(QObject *parent)
    : TreeModel(parent)
{
}

bool DragDropModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    int k;

    int clF;
    int adF;
    int clR;
    int adR;
    int clD;
    int adD;

//    k = QMessageBox::question(0, "Send", "Are you're shure?", QMessageBox::Yes | QMessageBox::No);
//    if(k == QMessageBox::No)
//    {
//        return false;
//    }

    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("text/plain"))
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = 0;
    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("text/plain");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QHash<qint64, QMap<int,QHash<int,QString> > > newItems;
    bool b = false;
    int rowTemp = -2;
    int colTemp = -2;
    int idTemp = -2;
    while (!stream.atEnd())
    {
        qint64 id;
        int row;
        int column;
        QString text;
        stream >> id >> row >> column >> text;
        newItems[id][row][column] = text;
        QModelIndex ind = createIndex(row, column, int(id));
        QModelIndex indP = ind.parent();

        if(idTemp!=id && ind.isValid())
        {
            clR = ind.model()->data(ind.model()->index(ind.row(), 2, ind.parent()), Qt::DisplayRole).toInt();
            adR = ind.model()->data(ind.model()->index(ind.row(), 1, ind.parent()), Qt::DisplayRole).toInt();

            clF = indP.model()->data(indP.model()->index(indP.row(), 2, indP.parent()), Qt::DisplayRole).toInt();
            adF = indP.model()->data(indP.model()->index(indP.row(), 1, indP.parent()), Qt::DisplayRole).toInt();

            QModelIndex ind1 = parent;

            if(ind1.isValid())
            {
                    clD = ind1.model()->data(ind1.model()->index(ind1.row(), 2, ind1.parent()), Qt::DisplayRole).toInt();
                    adD = ind1.model()->data(ind1.model()->index(ind1.row(), 1, ind1.parent()), Qt::DisplayRole).toInt();               
            }
            else
            {
                clD = 1;
                adD = 1;
            }

            if(ind.parent() == ind1)
                return false;
            else
            {

                emit droppedDev(clR, adR, clD, adD, clF, adF);
                return true;
            }

            //removeRows(row, 1, ind.parent());
        }
        b = true;
        rowTemp = row;
        colTemp = column;
        idTemp = id;
    }    

    int rows = newItems.count();    
    insertRows(beginRow, rows, parent);    
    QMap<int,QHash<int,QString> > childItems;
    foreach (childItems, newItems.values())
    {
        QHash<int,QString> rowItems;
        foreach (rowItems, childItems.values())
        {
            foreach (int column, rowItems.keys())
            {
                QModelIndex idx = index(beginRow, column, parent);                
                setData(idx, rowItems[column]);                
            }
            ++beginRow;
        }
    }

    return true;
}

Qt::ItemFlags DragDropModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = TreeModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QMimeData *DragDropModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid())
        {
            if(index.parent() == QModelIndex() && index.model()->data(index.model()->index(index.row(),
                    1, index.parent()), Qt::DisplayRole) == 0
                    && index.model()->data(index.model()->index(index.row(), 2, index.parent()), Qt::DisplayRole) == 0)
            {
                QMessageBox::critical(0, "ERROR", "Couldnt move coordinator", QMessageBox::Yes);
                return NULL;
            }
            QString text = data(index, Qt::DisplayRole).toString();            
            int temp = index.internalId();
            stream << index.internalId() << index.row() << index.column() << text;

        }
    }    
    mimeData->setData("text/plain", encodedData);
    //mimeData->data()
    return mimeData;
}

QStringList DragDropModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

Qt::DropActions DragDropModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void DragDropModel::saveindex(QModelIndex ind)
{
    stst = ind;
}
