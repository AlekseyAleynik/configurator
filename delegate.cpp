#include "delegate.h"

Delegate::Delegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex & index) const
{
    /*QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);*/
    QComboBox *editor = new QComboBox(parent);
    QPalette pal = editor->palette();
    pal.setColor(QPalette::HighlightedText, QColor(Qt::white));
    editor->setPalette(pal);
    //editor->addItems((QStringList() << list_defTcontrol));
    return editor;
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    /*int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);*/
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *cmbBox = static_cast<QComboBox*>(editor);
    cmbBox->setCurrentIndex(cmbBox->findText(value, Qt::MatchCaseSensitive));
    temp_editor = cmbBox;
    //connect(cmbBox, SIGNAL(valueChanged(int)), this, SLOT(indexChanged(int)));
    connect(cmbBox, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
    connect(cmbBox, SIGNAL(editTextChanged(QString)), this, SLOT(editTextChangedSlot(QString)));
}

void Delegate::editTextChangedSlot(QString new_val)
{
    disconnect(temp_editor, SIGNAL(editTextChanged(QString)), this, SLOT(editTextChangedSlot(QString)));
    emit signal_changed(ind_field, new_val);
}

void Delegate::indexChanged(int ind_newCB)
{
    if(ind_field.column() == 7)
        emit signal_changed(ind_field, QString::number(ind_newCB));
    else
    {
        disconnect(temp_editor, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
        QString new_val = temp_editor->itemData(ind_newCB, Qt::DisplayRole).toString();
        emit signal_changed(ind_field, new_val);
    }
}

/*void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}*/

void Delegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 6 && index.data().toString() > 0)
    {
        QStyleOptionComboBox cmb;
        /*if(!devType(index) || !stopFlag)
            cmb.editable = false;*/
        cmb.rect = option.rect;
        cmb.popupRect = option.rect;
        cmb.currentText = index.data().toString();
        cmb.state = QStyle::State_Enabled;
        cmb.subControls = QStyle::SC_All;
        if(index.data(Qt::TextColorRole) == QColor(Qt::gray))
            cmb.palette.setColor(QPalette::Text, QColor(Qt::gray));
        QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &cmb, painter);
        QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &cmb, painter);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

/*bool Delegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                           const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        emit signal_changed(index);
        qApp->processEvents();
    }
    qApp->processEvents();
}*/

void Delegate::delayFunc(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int progress = index.data().toInt();
    QStyleOptionProgressBar progressBarOption;
    progressBarOption.rect = option.rect;
    progressBarOption.minimum = 0;
    QString s = index.model()->data(index.model()->index(index.row(), 8), Qt::DisplayRole).toString();
    if(s.size() < 3)
        progressBarOption.maximum = 100;
    else
    {
        QString temp = s.right(3);
        s = s.left(s.size() - 4);
        if(temp.contains("sec"))
            progressBarOption.maximum = s.toInt();
        else
            progressBarOption.maximum = s.toInt() * 60;
    }
    progress = progressBarOption.maximum - progress;
    progressBarOption.progress = progress;
    if(index.data().toInt())
        progressBarOption.text = QString::number(index.data().toInt()) + "sec";
    else
        progressBarOption.text = "Нет связи";
    progressBarOption.textVisible = true;
    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}
