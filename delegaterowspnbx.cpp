#include "delegaterowspnbx.h"

DelegateRowSpnBx::DelegateRowSpnBx(My_table *_tbl)
{
    tbl = _tbl;
}

QWidget *DelegateRowSpnBx::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);
    QPalette pal = editor->palette();
    pal.setColor(QPalette::HighlightedText, QColor(Qt::white));
    editor->setPalette(pal);
    return editor;
}

void DelegateRowSpnBx::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ind_field = index;
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spnBx = static_cast<QSpinBox*>(editor);
    spnBx->setValue(value);
    connect(spnBx, SIGNAL(valueChanged(int)), this, SLOT(indexChanged(int)));
}

void DelegateRowSpnBx::indexChanged(int ind_newCB)
{
    emit signal_changed(ind_field, QString::number(ind_newCB));
}

void DelegateRowSpnBx::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
