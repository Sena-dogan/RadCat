#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QUrl>

#include "../LogicThread.hpp"
#include "AppController.hpp"
#include <QQmlContext>
#include <QQuickStyle>

void registerQmlTypes() {
  qmlRegisterSingletonType<AppController>(
      "RadCat", 1, 0, "AppController",
      [](QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new AppController();
      });
}

int main(int argc, char *argv[]) {
  QQuickStyle::setStyle("Basic");
  QGuiApplication app(argc, argv);

  registerQmlTypes();

  QQmlApplicationEngine engine;

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.load(QUrl("qrc:/RadCat/src/UI/Main.qml"));

  return app.exec();
}
