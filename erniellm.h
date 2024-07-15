#ifndef ERNIELLM_H
#define ERNIELLM_H

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

class ernieLLM : public QObject
{
    Q_OBJECT

public:
    explicit ernieLLM(QObject *parent = nullptr);
    QString getAccessToken();
    QString chatErnie(const QString &content);

private:
    QNetworkAccessManager *networkManager;
    QString clientId;
    QString client_secret;
};

#endif // ERNIELLM_H
