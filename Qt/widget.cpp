#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget * parent) : QWidget(parent), ui(new Ui::Widget)
{
    remoteServer = new QWebSocketServer("Remote Server", QWebSocketServer::NonSecureMode, this);

    if (remoteServer->listen(QHostAddress::Any, 9999)) {
        qDebug() << "Remote control socket started listening in port 9999!";

        connect(remoteServer, &QWebSocketServer::newConnection, this, &Widget::clientConnect);
    }
    else {
        qDebug() << "Failed to start socket! " << remoteServer->errorString();
    }

    int id = QFontDatabase::addApplicationFont("Fonts/ProgressivePixel.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(family);
    pixelFont.setStyleStrategy(QFont::PreferAntialias);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint /* | Qt::Tool */);

    int screenW = QApplication::desktop()->width();
    int screenH = QApplication::desktop()->height();

    this->move(screenW / 2 - 200, screenH / 2 - 200);

    ui->setupUi(this);

    gif = new QMovie("donut.gif");
    gif->setScaledSize(QSize(150, 150));
    // gif->setSpeed(10);

    QGraphicsDropShadowEffect * gifShadow = new QGraphicsDropShadowEffect(this);
    gifShadow->setBlurRadius(10);
    gifShadow->setColor(QColor(0, 0, 0));
    gifShadow->setOffset(0, 0);

    gifLabel = new QLabel(this);
    gifLabel->setGraphicsEffect(gifShadow);
    gifLabel->setGeometry(125, 40, 150, 150);
    gifLabel->setMovie(gif);
    gifLabel->show();

    QGraphicsDropShadowEffect * infoShadow = new QGraphicsDropShadowEffect(this);
    infoShadow->setBlurRadius(10);
    infoShadow->setColor(QColor(0, 0, 0));
    infoShadow->setOffset(0, 0);

    donutInfo = new QLabel(this);
    donutInfo->setFont(pixelFont);

    donutInfo->setStyleSheet("border: 0px solid white; color: white; font-size: 22px");
    donutInfo->setAlignment(Qt::AlignHCenter);
    donutInfo->setGeometry(40, 200, 300, 30);
    donutInfo->setGraphicsEffect(infoShadow);
    donutInfo->show();

    QGraphicsDropShadowEffect * messageShadow = new QGraphicsDropShadowEffect(this);
    messageShadow->setBlurRadius(20);
    messageShadow->setColor(QColor(0, 0, 0));
    messageShadow->setOffset(0, 0);

    donutMessage = new QLabel(this);
    donutMessage->setFont(pixelFont);
    donutMessage->setStyleSheet("border: 0px solid white; color: white; font-size: 16px; line-height: 16px;");
    donutMessage->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    donutMessage->setWordWrap(true);
    donutMessage->adjustSize();
    donutMessage->setGeometry(40, 240, 300, 120);
    donutMessage->setGraphicsEffect(messageShadow);
    donutMessage->show();

    this->setWindowOpacity(0);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::donutPopup(Donut donation) {
    if (isPopedUp) return;

    isPopedUp = true;

    donationsList.pop();

    donutInfo->setText(donation.nickname + " - " + QString::number(donation.sum) + " " + donation.getCurrencyName());
    donutMessage->setText(donation.message);

    QSoundEffect * effect = new QSoundEffect(this);
    effect->setSource(QUrl::fromLocalFile("Alert.wav"));
    effect->setLoopCount(1);
    effect->play();

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

    QTimer::singleShot(1500, [&]() {
        isPopedUp = false;
        gif->stop();

        if (donationsList.size() > 0)
            donutPopup(donationsList.front());
    });
}
void Widget::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Space) {
        Donut temp;
        temp.message = "There is your message!";
        temp.nickname = "Hello World!";
        temp.sum = 124;
        temp.currency = Donut::RUB;

        donutPopup(temp);
    }
}

void Widget::getDonation (const QString & donationJSON) {
    qDebug() << donationJSON;

    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(donationJSON.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object";
        }
    }

    Donut temp;
    temp.nickname = obj.value("nick").toString();
    temp.message = obj.value("message").toString();
    temp.sum = obj.value("sum").toString().toFloat();
    temp.currency = Donut::RUB;

    donationsList.push(temp);

    if (donationsList.size() == 1)
        donutPopup(donationsList.front());
}

void Widget::clientConnect () {
    qDebug() << "New connection";

    auto socket = remoteServer->nextPendingConnection();
    socket->setParent(this);

    connect(socket, &QWebSocket::textMessageReceived, this, &Widget::getDonation);
    // connect(socket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);

    clients << socket;
}
