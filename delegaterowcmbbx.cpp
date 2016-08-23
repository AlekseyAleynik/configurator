#include "delegaterowcmbbx.h"

DelegateRowCmbBx::DelegateRowCmbBx(My_table *_tbl)
{
    tbl = _tbl;
}

QWidget *DelegateRowCmbBx::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    QPalette pal = editor->palette();
    pal.setColor(QPalette::HighlightedText, QColor(Qt::white));
    editor->setPalette(pal);
    switch (index.column()) {
    case 11: // Пороги срабатывания Грань-РК
        if(index.model()->data(index.model()->index(index.row(), 52), Qt::DisplayRole).toInt()) {
            editor->addItem("3°");
            editor->addItem("10°");
            editor->addItem("25°");
            editor->addItem("45°");
        }
        else {
            editor->addItem("1-min");
            editor->addItem("2");
            editor->addItem("3");
            editor->addItem("4");
            editor->addItem("5");
            editor->addItem("6");
            editor->addItem("7");
            editor->addItem("8-max");
        }
        return editor;
    case 12: // Верхний порог срабатывания аналоговых ОУ
        for(int i = -53; i <= 123; i++)
            editor->addItem(QString::number(i));
        if(index.data(Qt::DisplayRole).toInt())
            editor->showMaximized();
        else
            editor->setCurrentIndex(editor->findData(index.data(Qt::DisplayRole)));
        return editor;
    case 13: // Нижний порог срабатывания аналоговых ОУ
        for(int i = -53; i <= 123; i++)
            editor->addItem(QString::number(i));
        if(index.data(Qt::DisplayRole).toInt())
            editor->showMinimized();
        else
            editor->setCurrentIndex(editor->findData(index.data(Qt::DisplayRole)));
        return editor;
    case 14: // Режим работы исполнительных устройств
        editor->addItem("выключено");
        editor->addItem("включено");
        editor->addItem("режим 0.5 Гц");
        editor->addItem("режим 1 Гц");
        editor->addItem("режим 2 Гц");
        editor->addItem("режим 4 Гц");
        /*for(int i = 6; i < 16; i++)
            editor->addItem(QString::number(i));
        editor->addItem("”знать все");*/
        return editor;
    case 15: // Время работы исполнительных устройств
        editor->addItem("без ограничений");
        editor->addItem("10 с");
        editor->addItem("20 с");
        editor->addItem("30 с");
        editor->addItem("1 мин");
        editor->addItem("5 мин");
        editor->addItem("10 мин");
        return editor;
    case 16: // Длительность активной фазы режима
        if(index.model()->data(index.model()->index(index.row(), 16), Qt::DisplayRole).toString() != "wait") {
            for(int i = 0; i < 1; i++)
                editor->addItem(QString::number(i * 100));
            return editor;
        }
    case 17: // Длительность пассивной фазы режима
        if(index.model()->data(index.model()->index(index.row(), 17), Qt::DisplayRole).toString() != "wait") {
            for(int i = 0; i < 1; i++)
                editor->addItem(QString::number(i * 100));
            return editor;
        }
    default: // 6-10 Вкл/Выкл
        editor->addItem("Вкл");
        editor->addItem("Выкл");
        return editor;
    }
}

void DelegateRowCmbBx::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
}

void DelegateRowCmbBx::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug() << "index.row()" << index.row();
    ind_field = index;
    QString value = index.data().toString();
    QComboBox *cmbBox = static_cast<QComboBox*>(editor);
    cmbBox->setCurrentIndex(cmbBox->findText(value, Qt::MatchCaseSensitive));
    //this->temp_editor = cmbBox;
    connect(cmbBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(editTextChangedSlot(QString)));
    connect(cmbBox, SIGNAL(editTextChanged(QString)), this, SLOT(editTextChangedSlot(QString)));
}

void DelegateRowCmbBx::paint(QPainter *_painter, const QStyleOptionViewItem &_option, const QModelIndex &index) const
{
    /*if(((index.column() < 11) && (!index.data().toString().isEmpty()))
            || ((index.column() == 11) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 1))
            || ((index.column() == 12) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 4))
            || ((index.column() == 13) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 4))
            || ((index.column() == 14) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2))
            || ((index.column() == 15) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2))
            || ((index.column() == 16) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2))
            || ((index.column() == 17) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2)))
    {
        QItemDelegate::paint(_painter, _option, index);
        QStyleOptionComboBox cmbBx;
        //if(!devType(index) || !stopFlag)
            //cmb.editable = false;
        cmbBx.rect = _option.rect;
        cmbBx.popupRect = _option.rect;
        cmbBx.currentText = index.data().toString();
        cmbBx.state = QStyle::State_Enabled;
        cmbBx.subControls = QStyle::SC_ComboBoxEditField;
        if(index.data(Qt::TextColorRole) == QColor(Qt::gray))
            cmbBx.palette.setColor(QPalette::Text, QColor(Qt::gray));
        //QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &cmbBx, _painter);
        QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &cmbBx, _painter);
        QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &cmbBx, _painter);
    }*/
    if((index.column() == 19) && (!index.data().toString().isEmpty())) {
        int progress = index.data().toInt();
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = _option.rect;
        progressBarOption.minimum = 0;
        int timeNum = index.model()->data(index.model()->index(index.row(), 28)).toInt();
        progressBarOption.maximum = timeNum * 60;
        progress = progressBarOption.maximum - progress;
        progressBarOption.progress = progress;
        if(index.data().toInt())
            progressBarOption.text = QString::number(index.data().toInt()) + "сек";
        progressBarOption.textVisible = true;
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, _painter);
    }
    if(((index.column() < 11) && (!index.data().toString().isEmpty()))
            || ((index.column() == 11) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 1))
            || ((index.column() == 12) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 4))
            || ((index.column() == 13) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 4))
            || ((index.column() == 14) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2))
            || ((index.column() == 15) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2))
            || ((index.column() == 16) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2))
            || ((index.column() == 17) && (index.model()->data(index.model()->index(index.row(), 35)).toInt() == 2)))
    {
        QItemDelegate::paint(_painter, _option, index);
        QStyleOptionButton btn;
        btn.rect = _option.rect;
        btn.text = index.data().toString();
        //btn.type = QStyleOptionButton::SO_ToolButton;
        //btn.state = QStyle::State_Enabled;
        //btn.palette.setColor(QPalette::Background, QColor(Qt::gray));
        QApplication::style()->drawControl(QStyle::CE_PushButton, &btn, _painter);
    }
}

/*void DelegateRowCmbBx::delayFunc(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
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
        if(temp.contains("сек"))
            progressBarOption.maximum = s.toInt();
        else
            progressBarOption.maximum = s.toInt() * 60;
    }
    progress = progressBarOption.maximum - progress;
    progressBarOption.progress = progress;
    if(index.data().toInt())
        progressBarOption.text = QString::number(index.data().toInt()) + "сек";
    progressBarOption.textVisible = true;
    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}*/

void DelegateRowCmbBx::editTextChangedSlot(QString new_val)
{
    if((ind_field.isValid()) && (ind_field.data().toString() != new_val))
        emit signalCellDataChanged(ind_field.row(), ind_field.column(), new_val, ind_field.data().toString());
    //disconnect(temp_editor, SIGNAL(editTextChanged(QString)), this, SLOT(editTextChangedSlot(QString)));
}

void DelegateRowCmbBx::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

bool DelegateRowCmbBx::editorEvent(QEvent *event, QAbstractItemModel *model,
                                  const QStyleOptionViewItem &option, const QModelIndex &index)
{
    qApp->processEvents();
}
