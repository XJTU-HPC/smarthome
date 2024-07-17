#ifndef SCREENSAVER_H
#define SCREENSAVER_H


#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScreen>

class screenSaver : public QWidget {
public:
    screenSaver() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setWindowState(Qt::WindowFullScreen);

        QLabel* label = new QLabel(this);
        label->setPixmap(QPixmap(":/background/background.png").scaled(QApplication::primaryScreen()->size(), Qt::KeepAspectRatioByExpanding));
        label->setAlignment(Qt::AlignCenter);
    }

    void showSaver() {
//        show();
    }

    void hideSaver() {
//        hide();
    }
};


#endif // SCREENSAVER_H
