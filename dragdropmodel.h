#ifndef DRAGDROPMODEL_H
#define DRAGDROPMODEL_H

#include "treemodel.h"

class DragDropModel : public TreeModel
{
    Q_OBJECT

public:
    DragDropModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);

    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions() const;
    QModelIndex stst;
    void saveindex(QModelIndex ind);

signals:
    void droppedDev(int clR, int addR, int clD, int addD, int clF, int adF);
};

#endif
