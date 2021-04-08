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
#include <QPropertyAnimation>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget * ui;

    QMovie * gif;
    QLabel * gifLabel;
    QLabel * donutInfo;
    QLabel * donutMessage;

    void donutPopup();
    void donutHide();

    void mousePressEvent (QMouseEvent * event);
};
#endif // WIDGET_H
