#include "LogicThread.hpp"
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "System.hpp"

void LogicManager::start() {
    system = new System();
    QTimer* timer = new QTimer(this); 
    connect(timer, &QTimer::timeout, this, &LogicManager::mainLoop); 
    timer->start(0); 
}

void LogicManager::stop() { QThread::currentThread()->quit();}


// --> QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
// This is for event procession around if it takes too long.

//
void LogicManager::mainLoop() {
    if (system->isRunning) {
        system->logic();
    } else {
        QThread::currentThread()->quit();
    }
}

void LogicManager::scanDevices() {
    if (!system) return;

    system->deviceHandler.deviceScan();

    QVariantList newList;
    const auto &devices = system->deviceHandler.foundDevices;

    for (const auto &dev : devices) {
        QVariantMap map;
        if (dev.deviceRegistryEntry) {
            map["deviceName"] = QString::fromStdString(
                dev.deviceRegistryEntry->deviceInfo.deviceName);
        } else {
            map["deviceName"] = "Unknown Device";
        }

        map["serialMatch"] = dev.matchData.serialMatch;
        map["nameMatch"] = dev.matchData.nameMatch;
        map["vidMatch"] = dev.matchData.vidMatch;
        map["pidMatch"] = dev.matchData.pidMatch;
        map["matchScore"] = dev.matchData.matchScore;
        map["connectionType"] = (int)dev.connectionType;

        newList.append(map);
    }

    // MOCK DATA FOR TESTING
    if (newList.isEmpty()) {
        QVariantMap mock1;
        mock1["deviceName"] = "Mock Gamma Detector";
        mock1["serialMatch"] = true;
        mock1["nameMatch"] = true;
        mock1["vidMatch"] = true;
        mock1["pidMatch"] = true;
        mock1["matchScore"] = 3;
        mock1["connectionType"] = 1; // LibUsb
        newList.append(mock1);

        QVariantMap mock2;
        mock2["deviceName"] = "Mock Spectrometer (Partial)";
        mock2["serialMatch"] = false;
        mock2["nameMatch"] = true;
        mock2["vidMatch"] = false;
        mock2["pidMatch"] = false;
        mock2["matchScore"] = 1;
        mock2["connectionType"] = 0; // FTDI
        newList.append(mock2);
    }

    if (newList != m_lastScanResults) {
        m_lastScanResults = newList;
        emit devicesFound(newList);
    }
}