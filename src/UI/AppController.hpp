#pragma once

#include "../LogicThread.hpp"
#include <QObject>
#include <QVariantList>

class AppController : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QVariantList foundDevices READ foundDevices NOTIFY foundDevicesChanged)

public:
  explicit AppController(LogicManager *logicManager, QObject *parent = nullptr);

  QVariantList foundDevices() const;

public slots:
  void scan();

signals:
  void foundDevicesChanged();

private:
  LogicManager *m_logicManager;
};
