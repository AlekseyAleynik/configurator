#ifndef DELEGATE_H
#define DELEGATE_H

#include <QApplication>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QComboBox>

static QModelIndex ind_field;
static QComboBox *temp_editor;

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    Delegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    //void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void delayFunc(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    void editTextChangedSlot(QString new_val);
    void indexChanged(int index);
private:
    /*bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index);*/
signals:
    void signal_changed(QModelIndex, QString newCB_val);
};

#endif
