#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget * parent) : QWidget(parent), ui(new Ui::Widget)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlag(Qt::FramelessWindowHint);

    qDebug() << QApplication::desktop()->width();

    int screenW = QApplication::desktop()->width();
    int screenH = QApplication::desktop()->height();

    this->move(screenW / 2 - 200, screenH / 2 - 200);

    ui->setupUi(this);

    gif = new QMovie("donut.gif");
    gif->setScaledSize(QSize(150, 150));

    gifLabel = new QLabel(this);
    gifLabel->setGeometry(125, 40, 150, 150);
    gifLabel->setMovie(gif);
    gifLabel->show();

    donutInfo = new QLabel(this);
    donutInfo->setText("Nickname - 300 RUB");
    donutInfo->setStyleSheet("border: 0px solid white; color: white; font-size: 22px");
    donutInfo->setAlignment(Qt::AlignHCenter);
    donutInfo->setGeometry(40, 200, 300, 30);
    donutInfo->show();

    donutMessage = new QLabel(this);
    donutMessage->setText("Hello dear streamer!");
    donutMessage->setStyleSheet("border: 0px solid white; color: white; font-size: 14px; line-height: 16px;");
    donutMessage->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    donutMessage->setWordWrap(true);
    donutMessage->setGeometry(40, 240, 300, 100);
    donutMessage->show();

    this->setWindowOpacity(0);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::donutPopup() {
    QSound::play(QDir::currentPath() + "/Alert.wav");

    QSoundEffect effect;
    effect.setSource(QUrl::fromLocalFile("Alert.wav"));
    effect.setVolume(1.0f);
    effect.play();

    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");

    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    gif->start();
    animation->start();

    QTimer::singleShot(5000, [=]() {
        donutHide();
    });
}
void Widget::donutHide() {
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");

    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
}

void Widget::mousePressEvent (QMouseEvent * event) {
    qDebug() << "Clicked!";

    if (event->button() == Qt::LeftButton)
        donutPopup();
}
