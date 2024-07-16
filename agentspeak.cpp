#include "agentspeak.h"

#include <QFileInfo>
agentSpeak::agentSpeak(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)), clientId("SEtbAwVsvhVGMFSs6JEhfARn"), client_secret("FWKt4MyNvyjDKIefvp4ac9zvBjy5aQLc")
{
}
agentSpeak::~agentSpeak()
{

}
QString agentSpeak::getAccessToken()
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

QString agentSpeak::speak(const QString &content)
{
    QString accessToken = getAccessToken();
    if (accessToken.isEmpty())
    {
        qWarning() << "Failed to get access token!";
        return QString();
    }

    QUrl url("https://tsn.baidu.com/text2audio");
    QUrlQuery query;
    query.addQueryItem("tex", content);
    query.addQueryItem("tok", accessToken);
    query.addQueryItem("cuid", "Tne0iAruAbf7M4beETHpf4hUzDH9Kb5H");
    query.addQueryItem("ctp", "1");
    query.addQueryItem("lan", "zh");
    query.addQueryItem("spd", "5");
    query.addQueryItem("pit", "5");
    query.addQueryItem("vol", "5");
    query.addQueryItem("per", "5");
    query.addQueryItem("aue", "3");

    QByteArray payload = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QEventLoop loop;
    QNetworkReply *reply = networkManager->post(request, payload);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray response = reply->readAll();
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    reply->deleteLater();

    if (contentType.startsWith("audio"))
    {
        QFile file("speak.mp3");
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(response);
            file.close();
            qDebug() << "MP3 file has been successfully saved as 'speak.mp3'.";
            return "speak.mp3";
        }
        else
        {
            qWarning() << "Failed to save MP3 file.";
            return QString();
        }
    }
    else
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject errorObj = jsonDoc.object();
        qWarning() << "An error occurred:" << errorObj;

        if (errorObj.contains("sn"))
        {
            qDebug() << "Debug SN:" << errorObj.value("sn").toString();
        }
        return QString();
    }
}

void agentSpeak::playAudio(const QString &filePath)
{
    QString program = "mpv";
    QStringList arguments;
    arguments << filePath;

    QProcess process;
    process.start(program, arguments);

    // Optionally, you can check if the process started successfully
    if (!process.waitForStarted()) {
        qDebug() << "Failed to start mpv process:" << process.errorString();
        return;
    }

    // Wait for the process to finish
    if (!process.waitForFinished(-1)) {
        qDebug() << "Failed to wait for mpv process to finish:" << process.errorString();
        return;
    }

    qDebug() << "mpv process finished with exit code" << process.exitCode() << "and exit status" << process.exitStatus();
}
