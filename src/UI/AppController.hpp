#include <QObject>
#include <QVariantList>

class AppController : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QVariantList foundDevices READ foundDevices NOTIFY foundDevicesChanged)

public:
  explicit AppController(QObject *parent = nullptr);

  QVariantList foundDevices() const;

public slots:
  void scan();
  void onDevicesFound(const QVariantList& devices);

signals:
  void foundDevicesChanged();
  void requestScan();

private:
  QVariantList m_foundDevices;
};
