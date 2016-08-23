#ifndef DIALOGSETNETPARS_H
#define DIALOGSETNETPARS_H

#include <QDialog>

namespace Ui {
class DialogSetNetPars;
}

class DialogSetNetPars : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetNetPars(QWidget *parent = 0);
    ~DialogSetNetPars();

private:
    Ui::DialogSetNetPars *ui;
};

#endif // DIALOGSETNETPARS_H
