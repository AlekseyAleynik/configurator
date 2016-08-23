#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::initAct()
{
    if(!ui->act_0->isChecked())
        tbl->hideColumn(0);
    if(!ui->act_1->isChecked())
        tbl->hideColumn(1);
    if(!ui->act_2->isChecked())
        tbl->hideColumn(2);
    if(!ui->act_3->isChecked())
        tbl->hideColumn(3);
    if(!ui->act_4->isChecked())
        tbl->hideColumn(4);
    if(!ui->act_5->isChecked())
        tbl->hideColumn(5);
    if(!ui->act_6->isChecked())
        tbl->hideColumn(6);
    if(!ui->act_7->isChecked())
        tbl->hideColumn(7);
    if(!ui->act_8->isChecked())
        tbl->hideColumn(8);
    if(!ui->act_9->isChecked())
        tbl->hideColumn(9);
    if(!ui->act_10->isChecked())
        tbl->hideColumn(10);
    if(!ui->act_11->isChecked())
        tbl->hideColumn(11);
    if(!ui->act_12->isChecked())
        tbl->hideColumn(12);
    if(!ui->act_13->isChecked())
        tbl->hideColumn(13);
    if(!ui->act_14->isChecked())
        tbl->hideColumn(14);
    if(!ui->act_15->isChecked())
        tbl->hideColumn(15);
    if(!ui->act_16->isChecked())
        tbl->hideColumn(16);
    if(!ui->act_17->isChecked())
        tbl->hideColumn(17);
    if(!ui->act_18->isChecked())
        tbl->hideColumn(18);
    if(!ui->act_19->isChecked())
        tbl->hideColumn(19);
    if(!ui->act_20->isChecked())
        tbl->hideColumn(20);
    if(!ui->act_21->isChecked())
        tbl->hideColumn(21);
    if(!ui->act_22->isChecked())
        tbl->hideColumn(22);
    if(!ui->act_23->isChecked())
        tbl->hideColumn(23);
    if(!ui->act_24->isChecked())
        tbl->hideColumn(24);
    if(!ui->act_25->isChecked())
        tbl->hideColumn(25);
    for(int i = numVisibleModelColumns + 1; i < mdl->columnCount(); i++)
        tbl->hideColumn(i);
    connect(ui->act_0, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_0(bool)));
    connect(ui->act_1, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_1(bool)));
    connect(ui->act_2, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_2(bool)));
    connect(ui->act_3, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_3(bool)));
    connect(ui->act_4, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_4(bool)));
    connect(ui->act_5, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_5(bool)));
    connect(ui->act_6, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_6(bool)));
    connect(ui->act_7, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_7(bool)));
    connect(ui->act_8, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_8(bool)));
    connect(ui->act_9, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_9(bool)));
    connect(ui->act_10, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_10(bool)));
    connect(ui->act_11, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_11(bool)));
    connect(ui->act_12, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_12(bool)));
    connect(ui->act_13, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_13(bool)));
    connect(ui->act_14, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_14(bool)));
    connect(ui->act_15, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_15(bool)));
    connect(ui->act_16, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_16(bool)));
    connect(ui->act_17, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_17(bool)));
    connect(ui->act_18, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_18(bool)));
    connect(ui->act_19, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_19(bool)));
    connect(ui->act_20, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_20(bool)));
    connect(ui->act_21, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_21(bool)));
    connect(ui->act_22, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_22(bool)));
    connect(ui->act_23, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_23(bool)));
    connect(ui->act_24, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_24(bool)));
    connect(ui->act_25, SIGNAL(triggered(bool)), this, SLOT(slotShowHideAct_25(bool)));
    connect(tbl, SIGNAL(signHideColumn(bool, int)), this, SLOT(slotHideActCommon(bool, int)));
}

void MainWindow::slotShowHideAct_0(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 0);
}

void MainWindow::slotShowHideAct_1(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 1);
}

void MainWindow::slotShowHideAct_2(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 2);
}

void MainWindow::slotShowHideAct_3(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 3);
}

void MainWindow::slotShowHideAct_4(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 4);
}

void MainWindow::slotShowHideAct_5(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 5);
}

void MainWindow::slotShowHideAct_6(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 6);
}

void MainWindow::slotShowHideAct_7(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 7);
}

void MainWindow::slotShowHideAct_8(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 8);
}

void MainWindow::slotShowHideAct_9(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 9);
}

void MainWindow::slotShowHideAct_10(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 10);
}

void MainWindow::slotShowHideAct_11(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 11);
}

void MainWindow::slotShowHideAct_12(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 12);
}

void MainWindow::slotShowHideAct_13(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 13);
}

void MainWindow::slotShowHideAct_14(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 14);
}

void MainWindow::slotShowHideAct_15(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 15);
}

void MainWindow::slotShowHideAct_16(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 16);
}

void MainWindow::slotShowHideAct_17(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 17);
}

void MainWindow::slotShowHideAct_18(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 18);
}

void MainWindow::slotShowHideAct_19(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 19);
}

void MainWindow::slotShowHideAct_20(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 20);
}

void MainWindow::slotShowHideAct_21(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 21);
}

void MainWindow::slotShowHideAct_22(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 22);
}

void MainWindow::slotShowHideAct_23(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 23);
}

void MainWindow::slotShowHideAct_24(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 24);
}

void MainWindow::slotShowHideAct_25(bool showHideFlag)
{
    tbl->showCol(showHideFlag, 25);
}

void MainWindow::slotHideActCommon(bool hideTrue, int currColumn)
{
    switch (currColumn) {
    case 0:
        ui->act_0->setChecked(!hideTrue);
        break;
    case 1:
        ui->act_1->setChecked(!hideTrue);
        break;
    case 2:
        ui->act_2->setChecked(!hideTrue);
        break;
    case 3:
        ui->act_3->setChecked(!hideTrue);
        break;
    case 4:
        ui->act_4->setChecked(!hideTrue);
        break;
    case 5:
        ui->act_5->setChecked(!hideTrue);
        break;
    case 6:
        ui->act_6->setChecked(!hideTrue);
        break;
    case 7:
        ui->act_7->setChecked(!hideTrue);
        break;
    case 8:
        ui->act_8->setChecked(!hideTrue);
        break;
    case 9:
        ui->act_9->setChecked(!hideTrue);
        break;
    case 10:
        ui->act_10->setChecked(!hideTrue);
        break;
    case 11:
        ui->act_11->setChecked(!hideTrue);
        break;
    case 12:
        ui->act_12->setChecked(!hideTrue);
        break;
    case 13:
        ui->act_13->setChecked(!hideTrue);
        break;
    case 14:
        ui->act_14->setChecked(!hideTrue);
        break;
    case 15:
        ui->act_15->setChecked(!hideTrue);
        break;
    case 16:
        ui->act_16->setChecked(!hideTrue);
        break;
    case 17:
        ui->act_17->setChecked(!hideTrue);
        break;
    case 18:
        ui->act_18->setChecked(!hideTrue);
        break;
    case 19:
        ui->act_19->setChecked(!hideTrue);
        break;
    case 20:
        ui->act_20->setChecked(!hideTrue);
        break;
    case 21:
        ui->act_21->setChecked(!hideTrue);
        break;
    case 22:
        ui->act_22->setChecked(!hideTrue);
        break;
    case 23:
        ui->act_23->setChecked(!hideTrue);
        break;
    case 24:
        ui->act_24->setChecked(!hideTrue);
        break;
    case 25:
        ui->act_25->setChecked(!hideTrue);
        break;
    default:
        return;
    }
}

void MainWindow::slotReShowCol(QModelIndex _ind)
{
    if(_ind.column() <= numVisibleModelColumns) {
        switch (_ind.column()) {
        case 0:
            if(ui->act_0->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 1:
            if(ui->act_1->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 2:
            if(ui->act_2->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 3:
            if(ui->act_3->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 4:
            if(ui->act_4->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 5:
            if(ui->act_5->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 6:
            if(ui->act_6->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 7:
            if(ui->act_7->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 8:
            if(ui->act_8->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 9:
            if(ui->act_9->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 10:
            if(ui->act_10->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 11:
            if(ui->act_11->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 12:
            if(ui->act_12->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 13:
            if(ui->act_13->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 14:
            if(ui->act_14->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 15:
            if(ui->act_15->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 16:
            if(ui->act_16->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 17:
            if(ui->act_17->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 18:
            if(ui->act_18->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 19:
            if(ui->act_19->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 20:
            if(ui->act_20->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 21:
            if(ui->act_21->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 22:
            if(ui->act_22->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 23:
            if(ui->act_23->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 24:
            if(ui->act_24->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        case 25:
            if(ui->act_25->isChecked()) {
                tbl->hideColumn(_ind.column());
                tbl->showColumn(_ind.column());
            }
            break;
        default:
            break;
        }
    }
}

void MainWindow::slotShowHideLogAct(bool showHideFlag)
{
    if(showHideFlag) {
        ui->txtEditSimpleLog->show();
        ui->lblEnentMess_txt->show();
        ui->chckBxNoize->show();
        ui->pbClear->show();
    }
    else {
        ui->txtEditSimpleLog->hide();
        ui->lblEnentMess_txt->hide();
        ui->chckBxNoize->hide();
        ui->pbClear->hide();
    }
}
