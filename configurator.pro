#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T16:10:11
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = configurator
TEMPLATE = app

include(./serialdevice/qserialdevice/qserialdevice.pri)
include(./serialdevice/qserialdeviceenumerator/qserialdeviceenumerator.pri)
include(./rkr-proto/rkr-proto.pri)

SOURCES += main.cpp\
    mainwindow.cpp \
    mymodel.cpp \
    dialogselectconnection.cpp \
    protocol.cpp \
    protocol485rk.cpp \
    protocolsigmark.cpp \
    global_variables.cpp \
    command.cpp \
    bootwidget.cpp \
    boot_protocol.cpp \
    vector2dint.cpp \
    my_table.cpp \
    form.cpp \
    log.cpp \
    logwidget.cpp \
    parse_comm.cpp \
    AboutDilog.cpp \
    dialogbinding.cpp \
    setnewnet.cpp \
    vector2droles.cpp \
    delegaterowcmbbx.cpp \
    delegaterowspnbx.cpp \
    devManagment.cpp \
    newcomm.cpp \
    show_hide.cpp \
    customcommands.cpp \
    treeitem.cpp \
    treemodel.cpp \
    treedev.cpp
    #sortfilterproxymdl.cpp

HEADERS  += mainwindow.h \
    mymodel.h \
    dialogselectconnection.h \
    protocol.h \
    protocol485rk.h \
    protocolsigmark.h \
    global_variables.h \
    command.h \
    bootwidget.h \
    boot_protocol.h \
    vector2dint.h \
    my_table.h \
    form.h \
    log.h \
    logwidget.h \
    AboutDilog.h \
    PortBaud.h \
    dialogbinding.h \
    setnewnet.h \
    vector2droles.h \
    delegaterowcmbbx.h \
    delegaterowspnbx.h \
    newcomm.h \
    treeitem.h \
    treemodel.h
    #sortfilterproxymdl.h

FORMS    += mainwindow.ui \
    dialogselectconnection.ui \
    logwidget.ui \
    AboutDilog.ui \
    dialogbinding.ui \
    logWidget.ui \
    setnewnet.ui

OTHER_FILES += \
    codEP.txt \
    CMD_USENS.txt \
    CMD_UINTERF.txt \
    CMD_UEXEC.txt \
    CMD_SYS.txt \
    serialdevice/qserialdeviceenumerator/qserialdeviceenumerator.pri \
    in_r.JPG \
    in_g.JPG \
    b.JPG \
    a.JPG \
    out_y.JPG \
    out_g.JPG \
    in_y.JPG \
    images/a.JPG \
    images/b.JPG \
    images/in_g.JPG \
    images/in_r.JPG \
    images/in_y.JPG \
    images/out_g.JPG \
    images/out_y.JPG \
    CMD_EXTRA.txt \
    CMD_SYS.txt \
    CMD_UEXEC.txt \
    CMD_UINTERF.txt \
    CMD_USENS.txt \
    CMD_UTEMPSENS.txt \
    codEP.txt


RESOURCES += \
    images.qrc \
    images.qrc

win32: {
    LIBS += -lsetupapi -luuid -ladvapi32
}

TRANSLATIONS += \
    main_ru.ts

CONFIG += static
