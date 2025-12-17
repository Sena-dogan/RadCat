#include "AppController.hpp"
#include "../Debug.hpp"
#include "../DeviceHandler.hpp"
#include "../System.hpp"

AppController::AppController(LogicManager *logicManager, QObject *parent)
    : QObject(parent), logicManager(logicManager) {

    // ---- Start backend logic thread
    logicManager = new LogicManager();
    logicManager->moveToThread(&logicThread);

    // ========================================================= CONNECTS HERE

    // When the logic thread starts, call LogicManager::start
    connect(&logicThread, &QThread::started, logicManager, &LogicManager::start);

    // Ensure proper shutdown on application exit
    connect(&app, &QGuiApplication::aboutToQuit, &appController, &AppController::shutdown);

    // Connect shutdown request signal from AppController to LogicManager stop slot
    connect(this, &AppController::requestShutdownOfLogicThread, logicManager, &LogicManager::stop);

    // Connect scan devices button clicked signal from UI to logic thread slot
    connect(this, &AppController::scanDevicesButtonClicked, logicManager, &LogicManager::scanDevicesButtonClicked);
    connect(logicManager, &LogicManager::devicesScanCompleted, this, &AppController::devicesScanCompletedHandler);








    logicThread.start();
    }

void AppController::scan() {
  if (!logicManager || !logicManager->system)
    return;

  Debug.Log("UI requested device scan...");
  logicManager->system->deviceHandler.deviceScan();
  emit foundDevicesChanged();
}

QVariantList AppController::foundDevices() const {
  QVariantList list;
  if (!logicManager || !logicManager->system)
    return list;

  const auto &devices = logicManager->system->deviceHandler.foundDevices;

  // MOCK DATA FOR TESTING
  if (devices.empty()) {
    QVariantMap mock1;
    mock1["deviceName"] = "Mock Gamma Detector";
    mock1["serialMatch"] = true;
    mock1["nameMatch"] = true;
    mock1["vidMatch"] = true;
    mock1["pidMatch"] = true;
    mock1["matchScore"] = 3;
    mock1["connectionType"] = 1; // LibUsb
    list.append(mock1);

    QVariantMap mock2;
    mock2["deviceName"] = "Mock Spectrometer (Partial)";
    mock2["serialMatch"] = false;
    mock2["nameMatch"] = true;
    mock2["vidMatch"] = false;
    mock2["pidMatch"] = false;
    mock2["matchScore"] = 1;
    mock2["connectionType"] = 0; // FTDI
    list.append(mock2);

    return list;
  }

  for (const auto &dev : devices) {
    QVariantMap map;
    // Basic info (assuming DeviceRegistryEntry has a deviceName, adjusting
    // based on actual structure)
    if (dev.deviceRegistryEntry) {
      map["deviceName"] = QString::fromStdString(
          dev.deviceRegistryEntry->deviceInfo.deviceName);
    } else {
      map["deviceName"] = "Unknown Device";
    }

    // Match info
    map["serialMatch"] = dev.matchData.serialMatch;
    map["nameMatch"] = dev.matchData.nameMatch;
    map["vidMatch"] = dev.matchData.vidMatch;
    map["pidMatch"] = dev.matchData.pidMatch;
    map["matchScore"] = dev.matchData.matchScore;

    // Add connection type if needed
    map["connectionType"] = (int)dev.connectionType;

    list.append(map);
  }
  return list;
}

void AppController::shutdown() {
    Debug.Log("Shutting down AppController and LogicManager...");
    if (logicManager) {
      emit requestShutdownOfLogicThread();
      logicThread.wait();
    }else{
      logicThread.quit();
      logicThread.wait();
    }
    Debug.Log("Shutdown complete.");
}

