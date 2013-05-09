#include "gamesession.h"
#include "city.h"
#include <QSettings>

#include <QDeclarativeView>

inline QString dataDir()
{
    QSettings ini(QSettings::IniFormat, QSettings::UserScope,
                  "CitySync","Client");
    return QFileInfo(ini.fileName()).absolutePath() + "/";
}

GameSession::GameSession(QObject *parent) :
    QObject(parent)
{

    //TODO: Load some sort of confifguration

    m_GameDir = "C:\\Steam\\steamapps\\common\\SimCity 4 Deluxe\\";
    m_GameExe = QDir::toNativeSeparators(dataDir()  + "SimCity 4.exe");
}


void GameSession::ensurePatchedExe()
{

  //  m_GameExe = m_GameDir + "Apps\\SimCity 4.exe";

    //TODO: check version, checksum etc

    QFile exeFile(":/resources/SimCity 4.exe");
    exeFile.copy(m_GameExe);

    QFile gameIni(dataDir() + "SimCity 4.ini");

    gameIni.open(QIODevice::WriteOnly);
    gameIni.write("[Directories]\r\n");
    gameIni.write("Data=");
    gameIni.write(m_GameDir.toAscii());
    gameIni.write("\r\n");

    gameIni.write("Plugins=");
    gameIni.write(m_GameDir.toAscii());
    gameIni.write("Plugins\\\r\n");

    gameIni.close();
}



void GameSession::playRegion(Region *region)
{

   m_Region = region;

   // const char* exe = "c:\\Windows\\notepad.exe";
    m_UserDirectory =  QDir::toNativeSeparators(region->dataDirectory().absolutePath() + "\\");
   createProcess();

   connect(&m_Watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChanged(QString)));
   connect(&m_Watcher,SIGNAL(directoryChanged(QString)),this,SLOT(directoryChanged(QString)));

   City* city;
   foreach(city, m_Region->cities())
   {
       m_Watcher.addPath(m_Region->saveDirectory().absoluteFilePath(city->currentFilename()));
   }

   m_Watcher.addPath(m_Region->saveDirectory().absolutePath());
}

void GameSession::fileChanged(QString file)
{
    if(!file.endsWith(".sc4"))
        return;

    QList<City*> cities = m_Region->cities().values();
    for(int i=0; i < cities.length(); i++)
    {
        if(cities[i]->currentFilename() == file)
        {
            //City updated
            cityUpdated(cities[i]);
            return;
        }
    }
}

void GameSession::directoryChanged(QString)
{
    QString name;
    foreach(name, m_Region->saveDirectory().entryList())
    {
        QString file = m_Region->saveDirectory().absoluteFilePath(name);

        if(file.endsWith(".sc4") && !m_Watcher.files().contains(file))
        {
            //New City?
            QString guid = City::extractGuid(file);
            City* city = m_Region->cities()[guid];
            city->updateFromFile(file);
            cityUpdated(city);

            m_Watcher.addPath(file);

        }
    }
}

void GameSession::cityUpdated(City* city)
{
    m_ModifiedCities.insert(city);
}


void GameSession::createOverlay()
{
    m_Overlay = new QDeclarativeView(QUrl("qrc:///resources/overlay.qml"));



    m_Overlay->setStyleSheet("background:transparent;");
    m_Overlay->setAttribute(Qt::WA_TranslucentBackground);
    m_Overlay->setWindowFlags(Qt::FramelessWindowHint);

    m_Overlay->setFixedSize(1680,1050);
    m_Overlay->move(0,0);
    m_Overlay->show();

}

void GameSession::createProcess()
{
    ensurePatchedExe();

    //QString workingDir = "C:\\Users\\Lisa\\CitySync\\build\\Sc4Hook\\debug";

    QString hookDll = "C:\\Users\\Lisa\\CitySync\\build\\Sc4Hook\\debug\\Sc4Hook.dll";

    QStringList args;

    args.append("-intro:off");

    args.append("-CustomResolution:enabled");
    args.append("-w");
    args.append("-r1024x768x32");

    args.append("-UserDir:" + m_UserDirectory);


   // args.append("-r1000x700x32");
    //args += "-FullScreenWindowed ";

    /*
    createOverlay();
    args.append("-OverlayHandle:" + QString::number((int)m_Overlay->winId()));
    */



    m_Proc.start(m_GameExe,args);

    connect(&m_Proc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished(int,QProcess::ExitStatus)));

#if 0
    QByteArray argsData = args.join(" ").toAscii();

    STARTUPINFOA startup = {0};
    startup.cb = sizeof(startup);

    BOOL result = ::CreateProcessA(
                        m_GameExe.toAscii().constData(),
                        argsData.data(),
                        NULL,
                        NULL,
                        TRUE,
                        CREATE_NEW_CONSOLE | CREATE_SUSPENDED,
                        NULL,
                        NULL,
                        &startup,
                        &m_GameProccess
                        );

    if(!result)
    {

        return;
    }



    // Copy the name of the hooking dll into remote memory
    void* dllNameRemote = VirtualAllocEx(m_GameProccess.hProcess,NULL,hookDll.length(),MEM_COMMIT, PAGE_READWRITE );
    ::WriteProcessMemory( m_GameProccess.hProcess, dllNameRemote, hookDll.toAscii().constData(), hookDll.length(), NULL );

    //Load the module
    HMODULE hKernel32 = GetModuleHandleA("Kernel32");
    HANDLE hHookThread = CreateRemoteThread( m_GameProccess.hProcess, NULL, 0,
                (LPTHREAD_START_ROUTINE) GetProcAddress( hKernel32, "LoadLibraryA" ),
                 dllNameRemote, 0, NULL );

    WaitForSingleObject( hHookThread, INFINITE );

    // Get handle of the loaded module
    HANDLE hHookModule;
    GetExitCodeThread( hHookThread, (DWORD*)&hHookModule );

    CloseHandle( hHookThread );
    VirtualFreeEx( m_GameProccess.hProcess, dllNameRemote, hookDll.length(), MEM_RELEASE );

    //Start the game
    ResumeThread(m_GameProccess.hThread);
#endif
}


void GameSession::finished(int exitCode, QProcess::ExitStatus exitStatus)
{

    City* city;
    foreach(city, m_ModifiedCities)
    {
    //    city->notifyGameUpdated();
    }

    emit gameExited();
}
