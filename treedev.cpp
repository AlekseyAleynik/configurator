#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::slotAskTree()
{
    treeMdl->clear();
    if(!treeMdl->insertRow(0))
        return;
    treeMdl->setData(treeMdl->index(0, 0), "Корень");
    unsigned char addr = 0;
    Command comm;
    for(int i = 0; i < 5; i++) {
        comm.setCommand(i, addr, 0x1C, &addr, 1);
        commLst.push_front(comm);
    }
}

void MainWindow::parce0x1a()
{
    qDebug() << "parce0x1a()";
}

void MainWindow::parce0x1b()
{
    qDebug() << "parce0x1b()";
}

void MainWindow::parce0x1c()
{
    unsigned char parAddress;
    QString devName, devSerial, devInfo;
    QVector<int> occupiedChans;
    int addr;
    if(currComm.numCh == 0) {
        for(int i = 0; i < numBytes0x15com; i++) {
            for(int j = 0; j < 8; j++) {
                if(currAnsw.param[i] & (int)pow(2, j)) {
                    parAddress = i * 8 + j + 1;
                    occupiedChans.append(parAddress);
                }
            }
        }
        for(int i = 0; i < arrOccupiedChans[currComm.numClass].size(); i++) {
            addr = arrOccupiedChans[currComm.numClass].at(i);
            if(occupiedChans.contains(addr)) {
                devName = mdl->mdlDt(mdl->rowMdl(addr, currComm.numClass), 27).toString();
                devSerial = mdl->mdlDt(mdl->rowMdl(addr, currComm.numClass), 26).toString();
                devInfo = devName + " (" + devSerial + ")";
                insertChild(treeMdl->index(0, 0), devInfo, addr, currComm.numClass);
            }
            else {
                parAddress = addr;
                Command comm(currComm.numClass, addr, 0x1C, &parAddress, 1);
                commLst.push_back(comm);
            }
        }
    }
    else {
        devName = mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 27).toString();
        devSerial = mdl->mdlDt(mdl->rowMdl(currComm.numCh, currComm.numClass), 26).toString();
        devInfo = devName + " (" + devSerial + ")";
        if((addr = findRouter()) > 0) {
            for(int i = 0; i < treeMdl->rowCount(treeMdl->index(0, 0)); i++) {
                if((treeMdl->data(treeMdl->index(i, 1, treeMdl->index(0, 0))).toInt() == addr) &&
                        (treeMdl->data(treeMdl->index(i, 2, treeMdl->index(0, 0))).toInt() == 0))
                    insertChild(treeMdl->index(i, 0, treeMdl->index(0, 0)), devInfo, addr, currComm.numClass);
            }
        }
    }
    ui->treeView->expandAll();
}

void MainWindow::insertChild(QModelIndex index, QString dev, int channel, int cls)
{
    if(!treeMdl->insertRow(0, index))
        return;
    treeMdl->setData(treeMdl->index(0, 0, index), dev);
    treeMdl->setData(treeMdl->index(0, 1, index), channel);
    treeMdl->setData(treeMdl->index(0, 2, index), cls);
    //if(!treeMdl->headerData(0, Qt::Horizontal).isValid())
        //treeMdl->setHeaderData(0, Qt::Horizontal, dev, Qt::EditRole);
    //if(!treeMdl->headerData(1, Qt::Horizontal).isValid())
        //treeMdl->setHeaderData(1, Qt::Horizontal, channel, Qt::EditRole);
    //if(!treeMdl->headerData(2, Qt::Horizontal).isValid())
        //treeMdl->setHeaderData(2, Qt::Horizontal, cls, Qt::EditRole);
    ui->treeView->selectionModel()->setCurrentIndex(treeMdl->index(0, 0, index),
                                                    QItemSelectionModel::ClearAndSelect);
}

void MainWindow::insertRow(QModelIndex index, QString dev, int channel, int cls)
{
    QModelIndex child;
    if(!treeMdl->insertRow(index.row() + 1, index.parent()))
        return;
    updateActions();
    child = treeMdl->index(index.row() + 1, 0, index.parent());
    treeMdl->setData(child, dev, Qt::EditRole);
    child = treeMdl->index(index.row() + 1, 0, index.parent());
    treeMdl->setData(child, channel, Qt::EditRole);
    child = treeMdl->index(index.row() + 1, 0, index.parent());
    treeMdl->setData(child, cls, Qt::EditRole);
}

void MainWindow::removeRow(QModelIndex index)
{
    treeMdl->removeRow(index.row(), index.parent());
}

void MainWindow::updateActions()
{
    QModelIndex rooInd = treeMdl->index(0, 0);
    ui->treeView->closePersistentEditor(rooInd);
}

int MainWindow::findRouter()
{
    for(int i = 0; i < numBytes0x15com; i++) {
        for(int j = 0; j < 8; j++) {
            if(currAnsw.param[i] & (int)pow(2, j))
                return i * 8 + j + 1;
        }
    }
    return 0;
}

void MainWindow::slotDropTreeNode(int routeAddr, int routeCls,
                                  int sourceAddr, int sourceCls, unsigned char commByte)
{
    if(routeCls != 0) {
        QMessageBox::critical(this, "Ошибка", "ОУ может быть передано только системному устройству",
                              QMessageBox::Ok);
        return;
    }
    ui->treeView->expandAll();
    unsigned char param[2];
    if(commByte == 0x1A)
        param[0] = routeAddr;
    else
        param[0] = broadcast_adr;
    param[1] = sourceAddr;
    Command comm(sourceCls, routeAddr, commByte, param, 2);
    commLst.push_front(comm);
}
