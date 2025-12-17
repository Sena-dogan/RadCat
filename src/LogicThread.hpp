#pragma once
#include <QObject>
#include <QVariantList>

class System;

class LogicManager : public QObject {
    Q_OBJECT
public:
    explicit LogicManager(QObject* parent = nullptr) : QObject(parent) { 
        // Only connections here ! No logic !

    }
System* system; //Core system instance

public slots:
void start();
void stop();
    void scanDevices(); 
    void mainLoop();

signals:
    void devicesFound(const QVariantList& devices);

private:
   QVariantList m_lastScanResults;
};