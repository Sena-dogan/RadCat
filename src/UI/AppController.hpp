#pragma once

#include "../LogicThread.hpp"
#include <QObject>
#include <QVariantList>
#include <QThread>
#include <vector>
#include "../DeviceHandler.hpp"

class AppController : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QVariantList foundDevices READ foundDevices NOTIFY foundDevicesChanged)

public:
  explicit AppController(LogicManager *logicManager, QObject *parent = nullptr);
  QVariantList foundDevices() const;


  //======== UI Functions
  

public slots:
  void scan();
  void shutdown();

  // Slot to handle device scan completion from LogicManager thread, this function takes found device info as parameter which is a vector
  void devicesScanCompletedHandler(std::vector<DeviceHandler::FoundDeviceInfoForUI> foundDeviceInfo);

signals:
  void foundDevicesChanged();

  //Signal to request logic thread to shutdown
  void requestShutdownOfLogicThread();

  //When any scan devices button is clicked in the UI send this signal to logic thread so that devices can be scanned
  void scanDevicesButtonClicked();

private:
  LogicManager* logicManager;
  QThread logicThread;
};
