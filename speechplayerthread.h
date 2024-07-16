#ifndef SPEECHPLAYERTHREAD_H
#define SPEECHPLAYERTHREAD_H

#include <QThread>
#include <QObject>
#include "agentspeak.h"  // 假设这是你实现语音播放的类

class speechPlayerThread : public QThread
{
    Q_OBJECT

public:
    explicit speechPlayerThread(const QString &text, agentSpeak *agentSpeak, QObject *parent = nullptr)
        : QThread(parent), text_(text), agentSpeak_(agentSpeak), stopRequested_(false) {}

signals:
    void startedPlaying();
    void finishedPlaying();

public slots:
    void requestStop()
    {
        stopRequested_ = true;
    }

protected:
    void run() override
    {
        emit startedPlaying();
        int start = 0;
        int length = 55;
        while (!stopRequested_ && start < text_.length())
        {
            QString segment = text_.mid(start, length);
            QString filepath = agentSpeak_->speak(segment);
            agentSpeak_->playAudio(filepath);
            start += length;
        }
        emit finishedPlaying();
    }

private:
    QString text_;
    agentSpeak *agentSpeak_;
    bool stopRequested_;
};

#endif // SPEECHPLAYERTHREAD_H
