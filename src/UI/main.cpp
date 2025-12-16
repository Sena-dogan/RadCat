#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "../LogicThread.hpp"
#include "AppController.hpp"
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
  QQuickStyle::setStyle("Basic");
  QGuiApplication app(argc, argv);

  // Initialize Logic
  LogicManager logicManager;
  logicManager.start();

  // Initialize AppController
  AppController appController(&logicManager);

  QQmlApplicationEngine engine;

  // Expose AppController to QML
  engine.rootContext()->setContextProperty("backend", &appController);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.load(QUrl("qrc:/RadCat/src/UI/Main.qml"));

  return app.exec();
}
