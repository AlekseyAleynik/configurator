#ifndef FORM_H
#define FORM_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <cmath>
#include <QTime>
#include <QMap>
#include <QLabel>
#include "stdlib.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextCodec>

class Form :  public QScrollArea
{
    Q_OBJECT
public:
    explicit Form(QMap<QString, int>, QWidget *parent = 0);
    ~Form();
    QPixmap *px;
    QLabel *lbl;
    QMap<QString, int> map;
    int w;
protected:
    void setLine(QPixmap *px);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
signals:
    void closeGraph();
private slots:
    void showPos(int);
private:
    int slider;
    int show_p;
};

#endif // FORM_H
