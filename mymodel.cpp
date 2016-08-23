#include "mymodel.h"

MyModel::MyModel(int _rows, int _cols, QObject *pobj) : QAbstractTableModel(pobj)
{
    numModelColumns = _cols;
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    vec = new Vector2dRoles(_rows, numModelColumns);
}

MyModel::MyModel(const MyModel &myMdl)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    vec = new Vector2dRoles(myMdl.rowCount(), myMdl.columnCount());
}

int MyModel::rowCount(const QModelIndex & /*parent*/) const
{
    return vec->str.size();
}

int MyModel::columnCount(const QModelIndex & /*parent*/) const
{
    return numModelColumns;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
        return vec->str[index.row()][index.column()];
    if(role == Qt::BackgroundColorRole)
        return vec->clr[index.row()][index.column()];
    return QVariant();
}

//! [quoting mymodel_e]
bool MyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
        emit cellDataChanged(index);
        vec->str[index.row()][index.column()] = value.toString();
        return true;
    }
    if(role == Qt::BackgroundColorRole) {
        vec->clr[index.row()][index.column()] = value.toString();
        return true;
    }
    return false;
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(((role == Qt::ToolTipRole) || (role == Qt::DisplayRole)) && (orientation == Qt::Horizontal)) {
        switch (section) {
        case 0:
            return QString("Сетевой\nадрес");
        case 1:
            return QString("Основное");
        case 2:
            if(role == Qt::ToolTipRole)
                return QString("Уровень\nсигнала");
            else
                break;
        case 3:
            return QString("Состяние\nОУ");
        case 4:
            return QString("Состояние\nканала");
        case 5:
            return QString("Параметры\nсвязи с ОУ");
        case 6:
            if(role == Qt::ToolTipRole)
                return QString("Кодирование\nданных");
            else
                break;
        case 7:
            return QString("Индикация\nопознавания");
        case 8:
            if(role == Qt::ToolTipRole)
                return QString("Звуковая\nиндикация\nсостояния");
            else
                break;
        case 9:
            if(role == Qt::ToolTipRole)
                return QString("Световая\nиндикация\nсостояния");
            else
                break;
        case 10:
            if(role == Qt::ToolTipRole)
                return QString("Охрана");
            else
                break;
        case 11:
            return QString("Пороги\nГрань-РК");
        case 12:
            return QString("Верхний\nпорог\nаналогового\nОУ");
        case 13:
            return QString("Нижний\nпорог\nаналогового\nОУ");
        case 14:
            return QString("Режим\nработы");
        case 15:
            return QString("Время\nработы");
        case 16:
            return QString("Время\nактивной\nфазы\nрежима");
        case 17:
            return QString("Время\nпассивной\nфазы\nрежима");
        case 18:
            return QString("Данные");
        case 19:
            return QString("Статус\nвыполниния\nкоманды");
        case 20:
            return QString("Связь");
        case 21:
            return QString("Резервное\nпитание");
        case 22:
            return QString("Основное\nпитание");
        case 23:
            return QString("Вскрытие");
        case 24:
            return QString("Запыление\nПД-РК");
        case 25:
            return QString("Неисправность\nдополнит.\nпитания");
            //--------------------------------------------------------------------
        case 26:
            return QString("Серийный\nномер");
        case 27:
            return QString("Имя");
        case 28:
            return QString("Период\nсвязи");
        case 29:
            return QString("Допустимое\nчисло\nпропусков");
        case 30:
            return QString("Период\nконтроля");
        case 31:
            return QString("Адрес 1-го\nканала ОУ");
        case 32:
            return QString("Адрес 2-го\nканала ОУ");
        case 33:
            return QString("Адрес 3-го\nканала ОУ");
        case 34:
            return QString("Адрес 4-го\nканала ОУ");
        case 35:
            return QString("Класс 1-го\nканала ОУ");
        case 36:
            return QString("Класс 2-го\nканала ОУ");
        case 37:
            return QString("Класс 3-го\nканала ОУ");
        case 38:
            return QString("Класс 4-го\nканала ОУ");
        case 39:
            return QString("Версия 1-го\nканала ОУ");
        case 40:
            return QString("Версия 2-го\nканала ОУ");
        case 41:
            return QString("Версия 3-го\nканала ОУ");
        case 42:
            return QString("Версия 4-го\nканала ОУ");
        case 43:
            return QString("Тип 1-го\nканала ОУ");
        case 44:
            return QString("Тип 2-го\nканала ОУ");
        case 45:
            return QString("Тип 3-го\nканала ОУ");
        case 46:
            return QString("Тип 4-го\nканала ОУ");
        case 47:
            return QString("Характеристика\n1-го канала ОУ");
        case 48:
            return QString("Характеристика\n2-го канала ОУ");
        case 49:
            return QString("Характеристика\n3-го канала ОУ");
        case 50:
            return QString("Характеристика\n4-го канала ОУ");
        case 51:
            return QString("Число каналов в\nмногоканальном ОУ");
        case 52:
            return QString("Номер канала в\nмногоканальном ОУ");
        case 53:
            return QString("Номер канала\nв классе");
        }
    }
    if((role == Qt::DecorationRole) && (orientation == Qt::Horizontal)) {
        switch (section) {
        case 2:
            return QPixmap(":/images/pics/sign_level.png");
        case 8:
            return QPixmap(":/images/pics/sound.png");
        case 9:
            return QPixmap(":/images/pics/lamp.png");
        case 10:
            return QPixmap(":/images/pics/guard.png");
        case 6:
            return QPixmap(":/images/pics/binarycode.png");
        }
    }
    return QVariant();
}

bool MyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    /*emit headerDataChanged(orientation, 0, 8);
    return true;*/
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    if(this->data(this->index(index.row(), 20), Qt::BackgroundColorRole) == QColor(Qt::red))
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else {
        if((index.column() > 5) && (index.column() < 17) && (!index.data().toString().isEmpty()))
            return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
        else
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
}

bool MyModel::insertRows(int row, int count, const QModelIndex &parent)
{// row=1, always
    beginInsertRows(parent, row, row + count - 1);
    vec->appendRow(QColor(Qt::white), "");
    endInsertRows();
    return true;
}

bool MyModel::removeRows(int row, int count, const QModelIndex &parent)
{// row=1, always
    beginRemoveRows(parent, row, row + count - 1);
    vec->delRow(row);
    endRemoveRows();
    return true;
}

QByteArray MyModel::if_multiChannel(int i)
{
    QByteArray a;
    for(int j = 0; j < rowCount(); j++) {
        if(data(index(i, 0)) == data(index(j, 0)))
            a.append(j + 1);
    }
    return a;
}

void MyModel::setMdlDt(int row, int col, QVariant val, int role)
{
    setData(index(row, col), val, role);
}

QVariant MyModel::mdlDt(int row, int col, int role)
{
    return data(index(row, col), role);
}

void MyModel::setMdlDt(QModelIndex ind, QVariant val, int role)
{

    setData(ind, val, role);
}

QVariant MyModel::mdlDt(QModelIndex ind, int role)
{
    return data(ind, role);
}

int MyModel::rowMdl(int addr, int numCls)
{
    for(int i = 0; i < rowCount(); i++) {
        if((mdlDt(i, 31, Qt::DisplayRole).toInt() == addr) && (mdlDt(i, 35, Qt::DisplayRole).toInt() == numCls))
            return i;
    }
    qDebug() << "rowMdl = -1";
    return -1;
}

/*QVariant MyModel::data1(const int _row, const int _col, int role) const
{
    if(role == Qt::DisplayRole)
        return vec->str[_row][_col];
    if(role == Qt::BackgroundColorRole)
        return vec->clr[_row][_col];
    return QVariant();
}

bool MyModel::setData1(const int _row, const int _col, const QVariant &value, int role)
{
    if((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
        emit cellDataChanged(this->index(_row, _col));
        vec->str[_row][_col] = value.toString();
        return true;
    }
    if(role == Qt::BackgroundColorRole) {
        vec->clr[_row][_col] = value.toString();
        return true;
    }
    return false;
}

QModelIndex MyModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    int id = -1;
    if(parent.isValid()) {
        id = (row + 1) << magicNum();
        id = id | parent.internalId();
    }
    else
        id = row;
    if(id != -1)
        return createIndex(row, column, id);
    else
        return QModelIndex();
}

int MyModel::magicNum() const
{
    return vec->str.size() % 2 + vec->str.size() / 2;
}
*/
