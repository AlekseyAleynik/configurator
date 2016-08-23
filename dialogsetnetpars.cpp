#include "dialogsetnetpars.h"
#include "ui_dialogsetnetpars.h"

DialogSetNetPars::DialogSetNetPars(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetNetPars)
{
    ui->setupUi(this);
}

DialogSetNetPars::~DialogSetNetPars()
{
    delete ui;
}
