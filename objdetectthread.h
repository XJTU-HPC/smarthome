#ifndef OBJDETECTTHREAD_H
#define OBJDETECTTHREAD_H

#include <QThread>

class ObjDetectThread : public QThread
{
public:
    explicit ObjDetectThread(QObject *parent = nullptr);
};

#endif // OBJDETECTTHREAD_H
