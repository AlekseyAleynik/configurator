#include "logwidget.h"
#include "ui_logwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include "QCalendarWidget"

logWidget::logWidget(QStringList list[5], QWidget *parent) : QWidget(parent), ui(new Ui::logWidget)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    ui->setupUi(this);
    log = new Log("log/Base.db");
    view = new QTableView;
    mdlPointerToDelete = 0;
    ui->gridLayout->addWidget(view);
    connect(ui->but, SIGNAL(clicked()), SLOT(button_slot()));
    connect(ui->clr_but, SIGNAL(clicked()), SLOT(clr_log()));
    connect(ui->close, SIGNAL(clicked()), SLOT(close_log()));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(slot_export()));
    QTime tbegin;
    tbegin.setHMS(0, 0, 0);
    ui->dateTimeEdit_1->setTime(tbegin);
    ui->dateTimeEdit_1->setCalendarPopup(true);
    ui->dateTimeEdit_2->setCalendarPopup(true);
    QCalendarWidget* cw = new QCalendarWidget();
    cw->setFirstDayOfWeek(Qt::Monday);
    QCalendarWidget* cw1 = new QCalendarWidget();
    cw->setFirstDayOfWeek(Qt::Monday);
    ui->dateTimeEdit_1->setCalendarWidget(cw);
    ui->dateTimeEdit_2->setCalendarWidget(cw1);
    QStringList s;
    s << "Восстановление"
      << "Нажата кнопка 1(Тревога)"
      << "Нажата кнопка 2(Взять)"
      << "Нажата кнопка 3(Снять)"
      << "Нажата кнопка 4(Тест)"
      << "Длинное нажатие кнопки 1(Тревога)"
      << "Длинное нажатие кнопки 2(Взять)"
      << "Длинное нажатие кнопки 3(Снять)"
      << "Длинное нажатие кнопки 4(Тест)"
      << "Восстановление"
      << "Сброс датчика"
      << "Нет питания датчика"
      << "Замыкание на общей"
      << "Нет корректных данных"
      << "Считанная температура вне диапазона -55/+55"
      << "Авария"
      << "Подключение"
      << "Тревога"
      << "Вскрыт"
      << "Закрыт"
      << "Запыление"
      << "Запыление устранено"
      << "Неисправность ОП"
      << "Норма ОП"
      << "Неисправность РП"
      << "Норма РП"
      << "Неисправность дополнительного питания"
      << "Норма дополнительного питания"
      << "Индикация опознавания"
      << "Индикация опознавания включена"
      << "Уровень сигнала"
      << "Обрыв"
      << "КЗ"
      << "Новый канал"
      << "Тип каннала"
      << "Кодирование "
      << "включен"
      << "выключен"
      << "Нарушение"
      << "Тревога по\nверхнему порогу"
      << "Тревога по\nнижнему порогу"
      << "Индикация состояния"
      << "Режим охраны"
      << "Звуковая индикация состояния"
      << "Установлен порог срабатывания"
      << "Установлены пороги срабатывания"
      << "Не подключен датчик"
      << "Не подключено питание датчика"
      << "КЗ между каналом и землей"
      << "КЗ между двумя каналами"
      << "Ошибка времени преобразования"
      << "Отключение датчика после преобразования"
      << "Ошибка CRC"
      << "Ошибка конфигурации датчика"
      << "Считанная температура не находиться в диапазоне от -55 до +125";
    ui->comboBox_net->addItems(s);
    //------------------Set Box-----------------------
    box_list.insert(ui->checkBox_0, ui->comboBox_1);
    box_list.insert(ui->checkBox_1, ui->comboBox_1);
    box_list.insert(ui->checkBox_2, ui->comboBox_2);
    box_list.insert(ui->checkBox_3, ui->comboBox_3);
    box_list.insert(ui->checkBox_4, ui->comboBox_4);
    box_list.insert(ui->checkBox_ch, ui->comboBox_ch);
    box_list.insert(ui->checkBox_net, ui->comboBox_net);
    box_list.insert(ui->checkBox_date, ui->comboBox_net);
    //-----------Setting Combos-----------------------
    ui->comboBox_0->addItems(list[0]);
    ui->comboBox_1->addItems(list[1]);
    ui->comboBox_2->addItems(list[2]);
    ui->comboBox_3->addItems(list[3]);
    ui->comboBox_4->addItems(list[4]);
    for(int i = 0; i < 124; i++) {
        if(i > 0)
            ui->comboBox_ch->insertItem(i, QString::number(i));
    }
    //box_list->setChecked(true);
}

logWidget::~logWidget()
{
    delete ui;
}

QString logWidget::getFilter()
{
    QString param, param1, param2, param_date, result;
    QMap<QCheckBox*, QComboBox*>::iterator it = box_list.begin();
    QCheckBox* temp;
    QComboBox* combo_temp;
    bool check = false;
    for(int i = 1; it != box_list.end(); i++, ++it) {
        temp = it.key();
        combo_temp = it.value();
        if(temp->isChecked()) {
            if(temp == ui->checkBox_0 || temp == ui->checkBox_1 || temp == ui->checkBox_2
                    || temp == ui->checkBox_3 || temp == ui->checkBox_4) {
                if(param.size() != 0)
                    param += "OR ";
                else {
                    int strNumCls;
                    if(temp == ui->checkBox_0)
                        strNumCls = 0;
                    if(temp == ui->checkBox_1)
                        strNumCls = 1;
                    if(temp == ui->checkBox_2)
                        strNumCls = 2;
                    if(temp == ui->checkBox_3)
                        strNumCls = 3;
                    if(temp == ui->checkBox_4)
                        strNumCls = 4;
                    param += "class=" + QString::number(strNumCls) + " ";
                    if(combo_temp->currentIndex()) {
                        int name_pos;
                        QString temp = combo_temp->currentText();
                        name_pos = combo_temp->currentText().lastIndexOf(";");
                        temp.remove(name_pos, temp.size() - name_pos);
                        param += "AND name='" + temp + "' ";
                    }
                }
            }
            if(temp == ui->checkBox_ch) {
                if(check && param1.size() == 0 && combo_temp->currentIndex() != 0)
                    param1 += "AND ";
                if(combo_temp->currentIndex()) {
                    param1 += "channel=" + combo_temp->currentText() + " ";
                    check = true;
                }
            }
            if(temp == ui->checkBox_net) {
                if(combo_temp->currentIndex()) {
                    if(check/* && param2.size()==0*/)
                        param2 += "AND ";
                    if((combo_temp->currentText() == "Тревога по\nверхнему порогу")
                            || (combo_temp->currentText() == "Тревога по\nнижнему порогу")
                            || combo_temp->currentText() == "Установлен порог срабатывания"
                            || combo_temp->currentText() == "Установлены пороги срабатывания"
                            || combo_temp->currentText() == "Уровень сигнала")
                        param2 += "signal LIKE '%" + combo_temp->currentText() + "%' ";
                    else
                        param2 += "signal='" + combo_temp->currentText() + "' ";
                    check = true;
                }
            }
            if(temp == ui->checkBox_date) {
                if(/*param.size()!=0 || */check)
                    param_date += "AND ";
                param_date += "time>='" + ui->dateTimeEdit_1->date().toString("yyyy-MM-dd") +
                        " " + ui->dateTimeEdit_1->time().toString("hh:mm:ss") + "' ";
                param_date += "AND ";
                param_date += "time<='" + ui->dateTimeEdit_2->date().toString("yyyy-MM-dd") +
                        " " + ui->dateTimeEdit_2->time().toString("hh:mm:ss") + "' ";
                check = true;
            }
        }
    }
    if(!ui->chckBx_Noize->isChecked())
        param += "signal NOT LIKE 'Помеха %' AND signal NOT LIKE 'Разница: %'";
    if(ui->chckBx_lvl->isChecked())
        param += "level<>0 ";
    QStringList lst_res;
    if(!param1.isEmpty())
        lst_res.push_back(param1);
    if(!param2.isEmpty())
        lst_res.push_back(param2);
    if(!param_date.isEmpty())
        lst_res.push_back(param_date);
    int first_string;
    if(!lst_res.isEmpty() && !param.isEmpty()) {
        param.insert(0, "AND (");
        param.insert(param.size(), ")");
    }
    QString str;
    for(int i = 0; i < lst_res.size(); i++) {
        str.append(lst_res.at(i));
        str.resize(4);
        if(!str.contains("AND") && !str.contains("OR")) {
            first_string = i;
            break;
        }
        str.clear();
    }
    if(lst_res.size() > 0) {
        result.append(lst_res.at(first_string));
        lst_res.removeAt(first_string);
    }
    for(int i = 0; i < lst_res.size(); i++)
        result.append(lst_res.at(i));
    return /*param+param1+param2+param_date*/ result + param;
}

void logWidget::button_slot()
{
    QString col = "name, netaddr, channel, class, signal, time, level ";
    QString par1 = getFilter();
    if(view->model() != 0)
        delete view->model();
    ui->gridLayout->addWidget(view);
    log->showTable(view, col, par1);
    view->setColumnWidth(4, 155);
    view->model()->rowCount();
}

void logWidget::clr_log()
{
    QString err = log->clearTable();
    if(err.size() > 0)
        QMessageBox::critical(this, "Database Error", err, QMessageBox::Yes);
    if(view->model())
        delete view->model();
}

void logWidget::close_log()
{
    if(view->model())
        delete view->model();
}

void logWidget::slot_log_clicked()
{
    QDate date = QDate::currentDate();
    ui->dateTimeEdit_2->setDate(date);
    QTime time = QTime::currentTime();
    ui->dateTimeEdit_2->setTime(time);
    date.setDate(date.year(), date.month(), date.day() - 1);
    ui->dateTimeEdit_1->setDate(date);
    //ui->dateTimeEdit_1->setTime(time);
}

void logWidget::slot_export()
{
    QString str = QFileDialog::getSaveFileName(this, "Export", "", "html file *.html ;; text file *.txt");
    if(str.isEmpty())
        return;
    QFile file(str);
    file.remove();
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::critical(this, "Ошибка", "Ошибка сохранения в указанный файл", QMessageBox::Yes);
    QByteArray buffer;
    QRegExp r(".*\.(txt|html)$");
    if(!r.exactMatch(str.toLower())) {
        QMessageBox::critical(this, "Ошибка", "Данный тип файлов не поддерживается!");
        return;//-->
    }
    QString fileType = r.cap(1);
    if(fileType == "txt")
        buffer.append(getTextData());
    else
        buffer.append(getHTMLData());
    file.write(buffer);
    file.close();
}

QByteArray logWidget::getTextData()
{
    QString OutStr;
    OutStr.append("Имя | Сетевой адрес | Канал | Класс | Сигнал | Время | Уровень сигнала\r\n");
    if(view->model() != 0) {
        QAbstractItemModel *modelExp = view->model();
        view->scrollTo(modelExp->index(0, 0));
        for(int i = 0; i < modelExp->rowCount(); i++) {
            for(int j = 0; j < modelExp->columnCount(); j++) {
                OutStr.append(modelExp->data(modelExp->index(i, j), Qt::DisplayRole).toString());
                OutStr.append("\t\t|\t\t");
            }
            OutStr.append("\r\n");
        }
    }
    return OutStr.toLocal8Bit();
}

QByteArray logWidget::getHTMLData()
{
    QString str;        
    str.clear();
    // Делаем заголовок страницы и начинаем таблицу
    str.append("<html>\n"
                    "<head>\n"
                        "<meta http-equiv=\"Content-Type\" Content=\"text/html; charset=UTF-8\">"
                        "<title>"
                            "Журнал прибора БРШС-РК-485"
                        "</title>"
                    "</head>");
    str.append("<body>\n"
                    "<table width=85% border=1>\n"
                        "<tr>"
                            "<th width=20%,nowrap>Имя</th>"
                            "<th width=20%,nowrap>Сетевой адрес</th>"
                            "<th width=34%,nowrap>Канал</th>"
                            "<th width=13%,nowrap>Класс</th>"
                            "<th width=13%,nowrap>Сигнал</th>"
                            "<th width=13%,nowrap>Время</th>"
                            "<th width=13%,nowrap>Уровень сигнала</th>"
                        "</tr>\n");
    if(view->model() != NULL) {
        QAbstractItemModel *modelExp = view->model();
        view->scrollTo(modelExp->index(0, 0));
        for(int i = 0; i < modelExp->rowCount(); i++) {
            for(int j = 0; j<modelExp->columnCount(); j++) {
                str.append("<td>");
                str.append(modelExp->data(modelExp->index(i, j), Qt::DisplayRole).toString());
                str.append("</td>");
            }
            str.append("</tr>\n");
        }
    }
    str.append("</table>\n"
            "</body>\n"
        "</html>\n");
    return str.toLocal8Bit();
}
