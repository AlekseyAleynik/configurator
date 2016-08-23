#ifndef DELEGATEROWCMBBX_H
#define DELEGATEROWCMBBX_H

#include <QObject>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QComboBox>
#include "my_table.h"

//static QModelIndex ind_field;
//static QComboBox *temp_editor;

//class DelegateRowCmbBx : public QStyledItemDelegate
class DelegateRowCmbBx : public QItemDelegate
{
    Q_OBJECT
public:
    DelegateRowCmbBx(My_table *_tbl);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *_painter, const QStyleOptionViewItem &_option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    void editTextChangedSlot(QString new_val);
private:
    My_table *tbl;
    //QModelIndex ind_field;
    //QComboBox *temp_editor;
    mutable QModelIndex ind_field;
    //mutable QComboBox *temp_editor;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                                      const QStyleOptionViewItem &option, const QModelIndex &index);
    //void delayFunc(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index);
signals:
    void signalCellDataChanged(int row, int col, QString newCB_val, QString oldCB_val);
};

#endif // DELEGATEROWCMBBX_H
