#ifndef FACERECTHREAD_H
#define FACERECTHREAD_H

#include <QThread>
#include <QMainWindow>
#include <iostream>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "TMtCNN.h"
#include "TArcface.h"
#include "TRetina.h"
#include "TWarp.h"
#include "TLive.h"
#include "TBlur.h"

class faceRecThread : public QThread
{
    Q_OBJECT
public:
    explicit faceRecThread(QObject *parent = nullptr);
    void run() override;
    void stop() { m_stop = true; }
    void DrawObjects(cv::Mat &frame, vector<FaceObject> &Faces);
private:
    bool p_stranger = false;
    bool n_stranger = false;
    int stranger_cnt = 0;
    volatile bool m_stop = false;
signals:
    void finished();
    void imageReady(const QImage &image);
    void alarm();
public slots:
    void startRecognition() {
            start();
        }

    void stopRecognition() {
            stop();
        }
};

#endif // FACERECTHREAD_H
