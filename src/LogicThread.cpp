#include "LogicThread.hpp"
#include <QTimer>
#include <QThread>
#include <QDebug>

void LogicManager::start() {
    QTimer* timer = new QTimer(this); 
    connect(timer, &QTimer::timeout, this, &LogicManager::mainLoop); 
    timer->start(0); 
}

void LogicManager::stop() { 
    

    QThread::currentThread()->quit();
}


// --> QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
// This is for event procession around if it takes too long.

//
void LogicManager::mainLoop() {
    if (system.isRunning) {
        system.logic();
    } else {
        QThread::currentThread()->quit();
    }
}


void LogicManager::scanDevicesButtonClicked() {
        system.deviceHandler.deviceScan();
        std::vector<DeviceHandler::FoundDeviceInfoForUI> foundDeviceInfo;
        for (const auto& device : system.deviceHandler.foundDevices) {
            foundDeviceInfo.push_back(device.matchData);
        }
        emit devicesScanCompleted(foundDeviceInfo);
}