#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class Region;

class Server : public QObject
{
    Q_OBJECT

public:
    Server(const QString& endpoint);
    ~Server();


    void login(const QString& username, const QString& password);

    void fetchRegions();

    const QString& url(){ return m_Endpoint; }

    const QMap<QString,Region*>& regions() { return m_Regions; }

    QNetworkReply* get(QNetworkRequest&);
    QNetworkReply* post(QNetworkRequest&, QIODevice&);

    QString username() { return "Black_Widow"; }

signals:



    void loginSuccess();
    void logginFailed();

    void regionsUpdated();

    void connectionFailure();



private slots:

    void handleGetRegionsResponse();

    void handleError(QNetworkReply::NetworkError);

private:
    void applyAuth(QNetworkRequest&);

    QNetworkReply* m_FetchRegionsRequest;
    QNetworkAccessManager m_Network;


    QString m_Endpoint;
    QString m_SessionId;

    QMap<QString,Region*> m_Regions;
};

#endif // SERVER_H
