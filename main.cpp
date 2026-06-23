#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "src/licensegenerator.h"

int main(int argc, char *argv[]) {
  QQuickStyle::setStyle("Fusion");

  QGuiApplication app(argc, argv);

  app.setOrganizationName("technovotum");
  app.setApplicationName("LicenseActivator");

  qmlRegisterType<LicenseGenerator>("License", 1.0, 0, "LicenseGenerator");

  QQmlApplicationEngine engine;

  engine.load(QUrl("qrc:/qt/qml/ActivatorUI/qml/Main.qml"));

  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}