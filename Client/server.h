#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class Region;

class Server : public QNetworkAccessManager
{
    Q_OBJECT

public:
    Server(const QString& endpoint);
    ~Server();


    void login(const QString& username, const QString& password);

    void fetchRegions();

    void applyAuth(QNetworkRequest&);

    const QString& url(){ return m_Endpoint; }

    const QMap<QString,Region*>& regions() { return m_Regions; }

signals:

    void loginSuccess();
    void logginFailed();

    void regionsUpdated();

    void connectionFailure();



private slots:

    void handleGetRegionsResponse();

private:

    QNetworkReply* m_FetchRegionsRequest;


    QString m_Endpoint;
    QString m_SessionId;

    QMap<QString,Region*> m_Regions;
};

#endif // SERVER_H
