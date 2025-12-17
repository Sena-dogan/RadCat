#pragma once
#include <QObject>
#include "System.hpp"
#include <vector>

class LogicManager : public QObject {
    Q_OBJECT
public:
    explicit LogicManager(QObject* parent = nullptr) : QObject(parent) { 
        // Only connections here ! No logic !

    }

System system;

public slots:
void start();
void stop();
void mainLoop();

// Slot to handle scan devices button clicked signal from UI
void scanDevicesButtonClicked();

signals:
// Signal to notify UI about completed device scan, passing found device info
void devicesScanCompleted(std::vector<DeviceHandler::FoundDeviceInfoForUI> foundDeviceInfo);

};