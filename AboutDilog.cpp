#include <QString>
#include <QDateTime>
#include "AboutDilog.h"
#include "ui_AboutDilog.h"

AboutDilog::AboutDilog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDilog)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    ui->setupUi(this);
    QString content;
    content.append("Программа <b>Конфигуратор Ладога-РК</b><br>");
    content.append("<b>(С) ЗАО \"Риэлта\"</b> г. Санкт-Петербург 2015<br>");
    content.append("Версия 2.0.25 (22.08.2016)<br>");
    content.append("<font color=blue>http://rielta.ru</font>");
    ui->_textLabel->setText(content);
    connect(ui->btn_lastChanges, SIGNAL(clicked()), this, SLOT(lastChangesSlot()));
}

AboutDilog::~AboutDilog()
{
    delete ui;
}

void AboutDilog::lastChangesSlot()
{
    //QString name = QString("file://%1/help.chm")
    QString name = QString("lastChanges.html").arg(QCoreApplication::applicationDirPath());
    //QString name = QString("help.chm");
    //if(!QDesktopServices::openUrl(QUrl::fromLocalFile(name)))
    if(!QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(name))))
        QMessageBox::critical(this, "Error", "Файл справки не найден", QMessageBox::Yes);
}
