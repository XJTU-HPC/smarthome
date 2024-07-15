#include "speechrecthread.h"
#include <QDebug>
#include <QFile>        // QFile头文件
#include <QTextStream>  // QTextStream头文件
#include <QIODevice>    // QIODevice头文件

speechRecThread::speechRecThread(QObject *parent)
    : QThread(parent), command(""), running(true)
{
    command = "./build/sherpa-ncnn-alsa ./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/tokens.txt "
            "./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/encoder_jit_trace-pnnx.ncnn.param "
            "./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/encoder_jit_trace-pnnx.ncnn.bin "
            "./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/decoder_jit_trace-pnnx.ncnn.param "
            "./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/decoder_jit_trace-pnnx.ncnn.bin "
            "./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/joiner_jit_trace-pnnx.ncnn.param "
            "./sherpa-ncnn-streaming-zipformer-zh-14M-2023-02-23/joiner_jit_trace-pnnx.ncnn.bin "
            "plughw:1,0";
}

speechRecThread::~speechRecThread()
{
    if (process->state() != QProcess::NotRunning) {
        process->kill();
        process->waitForFinished();
    }
    delete process;
}

void speechRecThread::run()
{
//    qDebug() << "speech run Thread ID:" << QThread::currentThreadId();
    process = new QProcess();

    // 连接信号
//    connect(process, &QProcess::readyReadStandardOutput, this, &speechRecThread::readProcessOutput);
//    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &speechRecThread::processFinished);

    // 重定向标准输出和标准错误到文件speechrec.txt
    QFile *outputFile = new QFile("speechrec.txt");
    if (!outputFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        // 打开文件失败, 可以添加错误处理
        delete outputFile;
        return;
    }

    process->setStandardOutputFile("speechrec.txt");
    process->setStandardErrorFile("speechrec.txt");
    // 启动进程
    process->start(command);
    // 等待进程运行并保持线程活动
    while (running && process->state() != QProcess::NotRunning) {
        this->readProcessOutput();
        msleep(500);  // 保持线程活动状态，检查running标志
    }
    outputFile->close();
    delete outputFile;
    if (process->state() != QProcess::NotRunning) {
        process->terminate();
        process->waitForFinished();
    }
}

void speechRecThread::stop()
{
    running = false;
//    qDebug() <<"good";
}

void speechRecThread::readProcessOutput()
{
//    qDebug() <<"start readProcessOutput";
    QFile file("speechrec.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 打开文件失败, 可以添加错误处理
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");  // 确保使用UTF-8编码
    QString output = in.readAll();
    file.close();
//    qDebug() <<output;
    emit outputReady(output);
}

void speechRecThread::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    running = false;
}
