#ifndef SPEECHRECTHREAD_H
#define SPEECHRECTHREAD_H

#include <QThread>
#include <QProcess>
#include <QString>

class speechRecThread : public QThread
{
    Q_OBJECT

public:
    explicit speechRecThread(QObject *parent = nullptr);
    ~speechRecThread();

    void run() override;
    void stop();

signals:
    void outputReady(QString output);

private slots:
    void readProcessOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *process;
    QString command;
    bool running;
};
#endif // SPEECHRECTHREAD_H
