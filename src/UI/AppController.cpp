#include "AppController.hpp"
#include "../Debug.hpp"

AppController::AppController(QObject *parent) : QObject(parent) {
  m_logicManager = new LogicManager();
  m_logicManager->moveToThread(&m_logicThread);

  // Connect Logic <-> UI

  // Logic Thread start, call start() on LogicManager
  QObject::connect(&m_logicThread, &QThread::started, m_logicManager,
                   &LogicManager::start);

  // Connect UI -> Logic: Scan button pressed
  QObject::connect(this, &AppController::requestScan, m_logicManager,
                   &LogicManager::scanDevicesButtonPressed);
  // Logic -> UI: Devices found
  QObject::connect(m_logicManager, &LogicManager::deviceScanCompleted, this,
                   &AppController::onDevicesFound);

  // Connect Shutdown signal to LogicManager
  QObject::connect(this, &AppController::requestShutdownOfLogicThread,
                   m_logicManager, &LogicManager::stop);

  // TODO: Exit

  // Start Logic Thread
  m_logicThread.start();
}

void AppController::scanButtonPressed() {
  Debug.Log("UI requested device scan...");
  emit requestScan();
}

void AppController::onDevicesFound(const QVariantList &devices) {
  m_foundDevices = devices;
  emit foundDevicesChanged();
}

QVariantList AppController::foundDevices() const { return m_foundDevices; }

void AppController::shutdown() {
  Debug.Log("UI requested shutdown...");
  if (m_logicManager) {
    emit requestShutdownOfLogicThread();
    m_logicThread.wait();
  } else {
    m_logicThread.quit();
    m_logicThread.wait();
  }
  Debug.Log("UI shutdown completed.");
}
