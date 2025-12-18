#pragma once
#include "System.hpp"
#include <QObject>
#include <QVariantList>

class LogicManager : public QObject {
  Q_OBJECT
public:
  explicit LogicManager(QObject *parent = nullptr) : QObject(parent) {
    // Only connections here ! No logic !
  }
  System system; // Core system instance

public slots:
  void start();
  void stop();
  void scanDevicesButtonPressed();
  void mainLoop();

signals:
  void deviceScanCompleted(const QVariantList &devices);

private:
  QVariantList m_lastScanResults;
};