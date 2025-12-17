#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QUrl>

#include "../LogicThread.hpp"
#include "AppController.hpp"
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
  QQuickStyle::setStyle("Basic");
  QGuiApplication app(argc, argv);

  // Initialize Logic
  // Initialize Logic (Worker)
  LogicManager *logicManager = new LogicManager();
  QThread *workerThread = new QThread();
  logicManager->moveToThread(workerThread);

  // Initialize AppController (UI)
  AppController appController;

  // Connect Logic <-> UI
  QObject::connect(workerThread, &QThread::started, logicManager, &LogicManager::start);
  QObject::connect(&appController, &AppController::requestScan, logicManager, &LogicManager::scanDevices);
  QObject::connect(logicManager, &LogicManager::devicesFound, &appController, &AppController::onDevicesFound);
  QObject::connect(&app, &QGuiApplication::aboutToQuit, [&]() {
      logicManager->stop(); // Tell logic to stop
      workerThread->quit(); // Tell thread to quit
      workerThread->wait(); // Wait for it
      delete logicManager;
      delete workerThread;
  });

  workerThread->start();

  QQmlApplicationEngine engine;

  // Expose AppController to QML
  engine.rootContext()->setContextProperty("backend", &appController);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.load(QUrl("qrc:/RadCat/src/UI/Main.qml"));

  return app.exec();
}
