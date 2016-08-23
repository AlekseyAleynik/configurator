#include <QtGui>
#include "treeitem.h"
#include "treemodel.h"

TreeModel::TreeModel(QVector<QVariant> &rootData, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(rootData);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if((!index.isValid()) || (role != Qt::DisplayRole))
        return QVariant();
    TreeItem *item = getItem(index);
    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if(index.isValid())
        return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsDropEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;
    bool result = rootItem->setData(section, value);
    if(result)
        emit headerDataChanged(orientation, section, section);
    return result;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem *parentItem = getItem(parent);
    TreeItem *childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;
    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();
    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();
    if(parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;
    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);
    if(result)
        emit dataChanged(index, index);
    return result;
}

void TreeModel::clear()
{
    for(int i = 0; i < this->rowCount(); i++) {
        if(removeRows(0, 1))
            i--;
    }
}

bool TreeModel::dropMimeData(const QMimeData *data,
                             Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    int routerAddr, routerCls, sourceAddr, sourceCls;
    //int clF, adF, clR, adR, clD, adD;
    if(action == Qt::IgnoreAction)
        return true;
    if(!data->hasFormat("text/plain"))
        return false;
    int beginRow;
    if(row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = 0;
    else
        beginRow = rowCount(QModelIndex());
    QByteArray encodedData = data->data("text/plain");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QHash<qint64, QMap<int, QHash<int, QString> > > newItems;
    while(!stream.atEnd()) {
        qint64 id;
        int rowS;
        int columnS;
        QString text;
        unsigned char commByte;
        stream >> id >> rowS >> columnS >> text;
        newItems[id][rowS][columnS] = text;
        QModelIndex ind = createIndex(rowS, columnS, int(id));
        QString tempRouterName, tempSourcesName;
        tempRouterName = parent.model()->data(parent.model()->index(parent.row(), 0, parent.parent()),
                                          Qt::DisplayRole).toString();
        tempSourcesName = ind.model()->data(ind.model()->index(ind.row(), 0, ind.parent()),
                                     Qt::DisplayRole).toString();
        routerAddr = parent.model()->data(parent.model()->index(parent.row(), 1, parent.parent()),
                                          Qt::DisplayRole).toInt();
        routerCls = parent.model()->data(parent.model()->index(parent.row(), 2, parent.parent()),
                                         Qt::DisplayRole).toInt();
        sourceAddr = ind.model()->data(ind.model()->index(ind.row(), 1, ind.parent()),
                                     Qt::DisplayRole).toInt();
        sourceCls = ind.model()->data(ind.model()->index(ind.row(), 2, ind.parent()),
                                    Qt::DisplayRole).toInt();
        if(ind.model()->index(ind.row(), columnS) == parent)
            commByte = 0x1B;
        else
            commByte = 0x1A;
        emit signalDropTreeNode(routerAddr, routerCls, sourceAddr, sourceCls, commByte);
        if(routerCls != 0)
            return false;
        removeRows(ind.row(), 1, ind.parent());
    }
    int rows = newItems.count();
    insertRows(beginRow, rows, parent);
    QMap<int, QHash <int, QString> > childItems;
    QHash<int, QString > rowItems;
    foreach (childItems, newItems.values()) {
        foreach (rowItems, childItems.values()) {
            foreach (int column, rowItems.keys()) {
                QModelIndex idx = index(beginRow, column, parent);
                setData(idx, rowItems[column]);
            }
            ++beginRow;
        }
    }
    return true;
}

QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    foreach (QModelIndex index, indexes) {
        if(index.isValid()) {
            if((index.parent() == QModelIndex()) && (index.model()->data(index.model()->index(index.row(),
                    1, index.parent())).toInt() == 0) && (index.model()->data(index.model()->index(index.row(),
                    2, index.parent())).toInt() == 0)) {
                QMessageBox::critical(0, "ERROR", "Couldnt move coordinator", QMessageBox::Yes);
                return NULL;
            }
            stream << index.internalId() << index.row() << index.column() << data(index).toString();
        }
    }
    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

QStringList TreeModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if(index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if(item)
            return item;
    }
    return rootItem;
}

QModelIndexList TreeModel::findIndexes(QString devInfo, int addrDev, int clsDev)
{
    TreeItem *item = rootItem;
    QModelIndex currIndex = this->index(0, 0);
    QModelIndexList indexLst;
    for(int i = 0; i < item->childCount(); i++) {
        currIndex = currIndex.child(i, 0);
        if((item->child(i)->data(0).toString() == devInfo) && (item->child(i)->data(1).toInt() == addrDev) &&
                (item->child(i)->data(2).toInt() == clsDev)) {
            indexLst.append(currIndex);
        }
    }
    return indexLst;
}
