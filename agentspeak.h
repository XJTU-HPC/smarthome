#ifndef AGENTSPEAK_H
#define AGENTSPEAK_H

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QFile>
#include <QProcess>

class agentSpeak : public QObject
{
    Q_OBJECT

public:
    explicit agentSpeak(QObject *parent = nullptr);
    ~agentSpeak();
    QString getAccessToken();
    QString speak(const QString &content);
    void playAudio(const QString &filePath);

private:
    QNetworkAccessManager *networkManager;
    QString clientId;
    QString client_secret;

};

#endif // AGENTSPEAK_H
