#ifndef READPE15THREAD_H
#define READPE15THREAD_H

#include <QThread>
class ReadPE15Thread : public QThread
{
     Q_OBJECT
public:
    ReadPE15Thread();
    virtual void run();
signals:
    void pesig();
};

#endif // READPE15THREAD_H

