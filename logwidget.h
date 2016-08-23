#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QtSql>
#include <QCheckBox>
#include <QMessageBox>
#include <QTextCodec>
#include "log.h"

namespace Ui
{
    class logWidget;
}

class logWidget : public QWidget
{
    Q_OBJECT
public:
    explicit logWidget(QStringList dev_list[5], QWidget *parent = 0);
    ~logWidget();
    Log *log;
private:
    Ui::logWidget *ui;
    QTableView *view;
    QMap<QCheckBox *, QComboBox *> box_list;
    QString getFilter();
    QAbstractItemModel *mdlPointerToDelete;
    QByteArray getTextData();
    QByteArray getHTMLData();
private slots:
    void button_slot();
    void clr_log();
    void close_log();
    void slot_log_clicked();
    void slot_export();
};

#endif // LOGWIDGET_H
