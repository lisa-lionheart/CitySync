#include "server.h"
#include "Region.h"

#include "json/json.h"
#include <QNetworkProxyFactory>


Server::Server(const QString& endpoint)
{
    m_Endpoint = endpoint;
    m_FetchRegionsRequest = NULL;

    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

Server::~Server()
{

}

void Server::handleError(QNetworkReply::NetworkError)
{

}

void Server::login(const QString &username, const QString &password)
{

}

QNetworkReply* Server::get(QNetworkRequest &r)
{
    applyAuth(r);
    QNetworkReply* reply = m_Network.get(r);

    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(handleError(QNetworkReply::NetworkError)));

    return reply;
}

QNetworkReply* Server::post(QNetworkRequest &r, QIODevice& dev)
{
    applyAuth(r);
    QNetworkReply* reply = m_Network.post(r,&dev);

    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(handleError(QNetworkReply::NetworkError)));

    return reply;
}

void Server::applyAuth(QNetworkRequest &)
{

}

void Server::handleGetRegionsResponse()
{
    QByteArray data = m_FetchRegionsRequest->readAll();

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( std::string(data.data(), data.length()) , root );
    if ( !parsingSuccessful )
    {
        return;
    }

    Json::Value cityData;
    foreach(cityData, root)
    {
        Region* region = new Region(this, cityData);
        m_Regions[region->name()] = region;
    }

    emit regionsUpdated();
}

void Server::fetchRegions()
{
    if(m_FetchRegionsRequest)
        return;

    QNetworkRequest fetchRegions( QUrl(m_Endpoint + "region") );
    applyAuth(fetchRegions);

    m_FetchRegionsRequest = get(fetchRegions);
    connect(m_FetchRegionsRequest,SIGNAL(finished()),SLOT(handleGetRegionsResponse()));
}


