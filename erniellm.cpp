#include "erniellm.h"

ernieLLM::ernieLLM(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)), clientId("TVEbN5bVmk2JQlfVRMgszcXH"), client_secret("Bk3JKtfQmPSCU3PhX0KoKwOwVgfh4Pal")
{
}

QString ernieLLM::getAccessToken()
{
    QUrl url("https://aip.baidubce.com/oauth/2.0/token");
    QUrlQuery query;
    query.addQueryItem("grant_type", "client_credentials");
    query.addQueryItem("client_id", clientId);
    query.addQueryItem("client_secret", client_secret);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error: " << reply->errorString();
        reply->deleteLater();
        return QString();
    }

    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        return jsonObj["access_token"].toString();
    }

    return QString();
}

QString ernieLLM::chatErnie(const QString &content)
{
    QString token = getAccessToken();
    if (token.isEmpty()) {
        return QString();
    }

    QUrl url(QString("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/ernie-3.5-8k-0205?access_token=%1").arg(token));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload;
    QJsonObject message;
    message["role"] = "user";
    message["content"] = content;
    QJsonArray messages;
    messages.append(message);
    payload["messages"] = messages;
    payload["temperature"] = 0.8;
    payload["top_p"] = 0.8;
    payload["penalty_score"] = 1;
    payload["disable_search"] = false;
    payload["enable_citation"] = false;
    payload["response_format"] = "text";

    QJsonDocument jsonDoc(payload);
    QByteArray jsonData = jsonDoc.toJson();

    QNetworkReply *reply = networkManager->post(request, jsonData);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error: " << reply->errorString();
        reply->deleteLater();
        return QString();
    }

    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonDocument responseDoc = QJsonDocument::fromJson(response);
    if (responseDoc.isObject()) {
        QJsonObject responseObj = responseDoc.object();
        return responseObj["result"].toString();
    }

    return QString();
}
