#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include <QProcess>
#include <QFileSystemWatcher>
#include <QSet>

#include "windows.h"
#include "region.h"
#include "city.h"


class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(QObject *parent = 0);
    
    void playRegion(Region* region);

    void createProcess();

signals:
    
    void gameExited();

public slots:
    

    void finished( int exitCode, QProcess::ExitStatus exitStatus );

    void fileChanged(QString);
    void directoryChanged(QString);

private:

    void cityUpdated(City*);
    void cityFounded();

    void createOverlay();
    void ensurePatchedExe();

    QSet<City*> m_ModifiedCities;

    Region* m_Region;

    QString m_GameDir;
    QString m_UserDirectory;
    QString m_GameExe;

    QWidget* m_Overlay;
    QProcess m_Proc;
    QFileSystemWatcher m_Watcher;

    PROCESS_INFORMATION m_GameProccess;
};

#endif // GAMESESSION_H
