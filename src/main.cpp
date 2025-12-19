#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "../LogicThread.hpp"
#include "AppController.hpp"


int main(int argc, char *argv[]) {
  QQuickStyle::setStyle("Basic");
  QGuiApplication app(argc, argv);


  QQmlApplicationEngine engine;

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  engine.loadFromModule("RadCat", "Main");

  return app.exec();
}
