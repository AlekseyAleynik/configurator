#ifndef ABOUTDILOG_H
#define ABOUTDILOG_H

#include <QDialog>
#include <QTabWidget>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextCodec>

namespace Ui
{
    class AboutDilog;
}

class AboutDilog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDilog(QWidget *parent = 0);
    ~AboutDilog();
private:
    Ui::AboutDilog *ui;
public slots:
    void lastChangesSlot();
};

#endif // ABOUTDILOG_H
