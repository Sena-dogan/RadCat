#include "LogicThread.hpp"
#include <QObject>
#include <QThread>
#include <QVariantList>

class AppController : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QVariantList foundDevices READ foundDevices NOTIFY foundDevicesChanged)

public:
  explicit AppController(QObject *parent = nullptr);

  QVariantList foundDevices() const;

public slots:
  void scanButtonPressed();
  void shutdown();
  void onDevicesFound(const QVariantList &devices);

signals:
  // TODO: Connect foundDevicesChanged later
  void foundDevicesChanged();
  void requestScan();
  void requestShutdownOfLogicThread();

private:
  QVariantList m_foundDevices;
  LogicManager *m_logicManager;
  QThread m_logicThread;
};
