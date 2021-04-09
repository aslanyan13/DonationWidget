#ifndef WIDGET_H
#define WIDGET_H

#include <QDir>
#include <QWidget>
#include <QMovie>
#include <QSound>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QScreen>
#include <QSoundEffect>
#include <QAudioDecoder>
#include <QAudioDeviceInfo>
#include <QApplication>
#include <QDesktopWidget>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QHostAddress>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include <QJsonObject>
#include <QJsonDocument>

#include <queue>

struct Donut {
public:
    enum Currencies {RUB, USD, AMD, EUR};

    QString nickname;
    QString message;

    float sum;

    Currencies currency;

    Donut() {
        nickname = "";
        message = "";
        sum = 0.0f;
    }

    QString getCurrencyName () {
        if (currency == RUB) return "RUB";
        if (currency == USD) return "USD";
        if (currency == AMD) return "AMD";
        if (currency == EUR) return "EUR";

        return "";
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void clientConnect ();
    void getDonation (const QString &);

private:
    Ui::Widget * ui;

    bool isPopedUp = false;

    std::queue <Donut> donationsList;

    QMovie * gif;
    QLabel * gifLabel;
    QLabel * donutInfo;
    QLabel * donutMessage;

    QWebSocketServer * remoteServer;

    void donutPopup(Donut donation);
    void donutHide();

    QList <QWebSocket *> clients;

    void keyPressEvent(QKeyEvent * event);
};
#endif // WIDGET_H
