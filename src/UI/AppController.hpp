#include "LogicThread.hpp"
#include <QObject>
#include <QThread>
#include <QVariantList>
#include <QQmlEngine>

class AppController : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit AppController(QObject *parent = nullptr);
  ~AppController() override;

  Q_PROPERTY(QVariantList foundDevices READ foundDevices NOTIFY foundDevicesChanged)
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
