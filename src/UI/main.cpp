#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "../LogicThread.hpp"
#include "AppController.hpp"
#include <QQmlContext>
#include <QQuickStyle>


void registerQMLTypes() {
  //Singleteon for Backend
  qmlRegisterSingletonType<AppController>(
    "App", 1, 0, "AppController",
    [](QQmlEngine*, QJSEngine*) -> QObject* {
        return new AppController;
    }
  );


}

int main(int argc, char *argv[]) {

  QQuickStyle::setStyle("Basic");

  QGuiApplication app(argc, argv);

  registerQMLTypes();

  QQmlApplicationEngine engine;

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  engine.load(QUrl("qrc:/RadCat/src/UI/Main.qml"));

  return app.exec();
}
