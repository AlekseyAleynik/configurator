#include "form.h"
#include <QToolTip>
#include <QMouseEvent>
#include "qmath.h"

Form::Form(QMap<QString, int> input, QWidget *parent) : QScrollArea(parent)
{
    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfCodec);
    QTextCodec::setCodecForCStrings(utfCodec);
    slider = 0;
    QVBoxLayout *box = new QVBoxLayout;
    map = input;
    lbl = new QLabel(this);
    show_p = map.size();
    px = new QPixmap(map.size() * 20, 130);
    px->fill(QColor(Qt::white));
    setLine(px);
    lbl->setPixmap(*px);
    box->addWidget(lbl);
    this->setWidget(lbl);
    this->setWindowFlags(Qt::Popup);
    connect(this->horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(showPos(int)));
    this->setMouseTracking(true);
    lbl->setMouseTracking(true);
}

Form::~Form()
{}

void Form::mousePressEvent(QMouseEvent *)
{
    this->close();
    emit closeGraph();
}

void Form::keyPressEvent(QKeyEvent *)
{
    this->close();
    emit closeGraph();
}

void Form::mouseMoveEvent(QMouseEvent *ev)
{
    int step = 10;
    int pl = (ev->pos().x() + slider + (step / 2)) / step;
    QMap<QString, int>::iterator it = map.begin();
    int tempS = 0, cc = 0;
    for(int i = 0; i < pl; i++) {
        if(0 < it.value() < 200) {
            tempS += it.value();
            cc++;
        }
        it++;        
    }
    if(0 < it.value() < 200) {
        tempS += it.value();
        cc++;
    }
    if(cc == 0)
        cc++;
    QPoint ps;
    QPoint r = this->pos();
    ps.setX(ev->pos().x() + 30);
    ps.setY(ev->pos().y() + 30);
    /*if((ev->pos().x() + slider) / 10 < map.size() - 1)
        QToolTip::showText(r, QString::number(it.value()) + "  |  " +
                           it.key() + " | \n" + "cреднее по каналу: " +
                           QString::number(tempS / cc));*/
    QMap<QString, int>::iterator it1 = map.begin();
    for(int i = 0; i < show_p; i++, it1++) {
        qreal x = i * px->width() / (px->width() / 10);
        qreal y = (px->height() - it1.value()) + 50;
        if(QMATH_H::qPow(ev->pos().x() - x, 2) + QMATH_H::qPow(ev->pos().y() - y, 2) < 25)
            QToolTip::showText(r, QString::number(it1.value()) + "  |  " +
                               it1.key() + " | \n" + "cреднее по каналу: " +
                               QString::number(tempS / cc));
    }
}

void Form::showPos(int i)
{
    slider = i;
    QPoint r = this->pos();
    QToolTip::showText(r, QString::number(i));
}

void Form::setLine(QPixmap *px)
{
    QPainter p(px);
    QMap<QString, int>::iterator it = map.begin();
    int i = 0;
    QPointF a[1500];
    QLinearGradient grad(0, 0, 0, 150);
    grad.setColorAt(0.5, QColor(Qt::red));
    grad.setColorAt(0.3, Qt::yellow);
    grad.setColorAt(0, QColor(Qt::green));
    //p.drawRect(0,0, 50, 200);
    p.setBrush(grad);
    for(int j = 0; j != show_p; it++, j++) {
        qreal x = i * px->width() / (px->width() / 10);
        qreal y = (px->height() - it.value()) + 50;
        a[i] = QPointF(x, y);
        p.drawEllipse(QPointF(x, y), 5, 5);
        i++;
    }
    p.drawPolyline(a, i);
}
