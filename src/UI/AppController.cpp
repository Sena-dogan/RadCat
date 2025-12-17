#include "AppController.hpp"
#include "../Debug.hpp"

AppController::AppController(QObject *parent)
    : QObject(parent) {}

void AppController::scan() {
  Debug.Log("UI requested device scan...");
  emit requestScan();
}

void AppController::onDevicesFound(const QVariantList& devices) {
    m_foundDevices = devices;
    emit foundDevicesChanged();
}

QVariantList AppController::foundDevices() const {
  return m_foundDevices;
}
