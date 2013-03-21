#include "server.h"
#include "Region.h"

#include "json/json.h"

Server::Server(const QString& endpoint)
{
    m_Endpoint = endpoint;
    m_FetchRegionsRequest = NULL;
}

Server::~Server()
{

}


void Server::login(const QString &username, const QString &password)
{

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

    for(int i=0; i < root.size(); i ++)
    {
        Region* region = new Region(this, root[i]);
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
