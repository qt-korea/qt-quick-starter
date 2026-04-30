#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "AppDataSource.h"
#include "AppDataViewModel.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("QtQuickStarter"));
    app.setOrganizationName(QStringLiteral("QtExample"));

    QQuickStyle::setStyle(QStringLiteral("Material"));

    QQmlApplicationEngine engine;

    auto *vm = engine.singletonInstance<AppDataViewModel *>("App.ViewModels", "AppDataViewModel");

    if (!vm) {
        qCritical() << "[main] Failed to obtain AppDataViewModel singleton";
        return -1;
    }

    // The data source is parented to the QGuiApplication so it lives for
    // the entire process and is destroyed in the right order at shutdown.
    vm->bindDataSource(new AppDataSource(&app));

    engine.loadFromModule("App.Integration", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[main] Failed to load QML root — aborting";
        return -1;
    }

    return app.exec();
}
