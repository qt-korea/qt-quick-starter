#include <QDir>
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

    const QString qmlDir =
        QDir(app.applicationDirPath()).absoluteFilePath(QStringLiteral("../qml"));
    engine.addImportPath(qmlDir);

    auto *vm = engine.singletonInstance<AppDataViewModel *>(
        "App.ViewModels", "AppDataViewModel");

    if (!vm) {
        qCritical() << "[main] Failed to obtain AppDataViewModel singleton."
                    << "QML import path was:" << qmlDir;
        return -1;
    }

    vm->bindDataSource(new AppDataSource(&app));

    engine.loadFromModule("App.Integration", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[main] Failed to load QML root — aborting."
                    << "QML import path was:" << qmlDir;
        return -1;
    }

    return app.exec();
}
