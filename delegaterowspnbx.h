#ifndef DELEGATEROWSPNBX_H
#define DELEGATEROWSPNBX_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <my_table.h>

//static QModelIndex ind_field;

class DelegateRowSpnBx : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DelegateRowSpnBx(My_table *_tbl);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    void indexChanged(int index);
signals:
    void signal_changed(QModelIndex, QString newCB_val);
private:
    My_table *tbl;
    mutable QModelIndex ind_field;
};

#endif // DELEGATEROWSPNBX_H
