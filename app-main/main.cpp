#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "AppDataSource.h"
#include "AppServices.h"
#include "ViewNavigator.h"
#include "View1ViewModel.h"
#include "View2ViewModel.h"
#include "View3ViewModel.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("QtQuickStarter"));
    app.setOrganizationName(QStringLiteral("QtExample"));

    QQuickStyle::setStyle(QStringLiteral("Material"));

    QQmlApplicationEngine engine;

    // ── Build the services bundle ────────────────────────────────────────
    //
    //   This is the SINGLE place where domain services are constructed
    //   and wired together. Each service is parented to the QGuiApplication
    //   so it lives for the entire process and is destroyed in the right
    //   order at shutdown.
    //
    //   To add a new service:
    //     1. Forward-declare it in core/AppServices.h
    //     2. Add a `Foo *foo = nullptr;` member to AppServices
    //     3. Add one allocation line below — that is all
    //
    //   No ViewNavigator change. No registerRoute<T>() change. No change
    //   to ViewModels that do not consume the new service.
    //
    AppServices services;
    services.dataSource = new AppDataSource(&app);
    // services.auth     = new AuthState(&app);     // future
    // services.settings = new SettingsRepo(&app);  // future

    // ── Hand the bundle to the navigator ─────────────────────────────────
    auto *navigator = engine.singletonInstance<ViewNavigator *>(
        "App.Core", "ViewNavigator");
    if (!navigator) {
        qCritical() << "[main] Failed to obtain ViewNavigator singleton";
        return -1;
    }
    navigator->setServices(services);

    // ── The route table — the single source of truth for navigation ──────
    //
    //   Add a new page = add a new line here + create the View*.qml under
    //   ui-kit/views/ + create the View*ViewModel under viewmodels/.
    //
    navigator->registerRoute<View1ViewModel>(
        QStringLiteral("View1"),
        QUrl(QStringLiteral("qrc:/qt/qml/App/UiKit/views/View1.qml")));

    navigator->registerRoute<View2ViewModel>(
        QStringLiteral("View2"),
        QUrl(QStringLiteral("qrc:/qt/qml/App/UiKit/views/View2.qml")));

    navigator->registerRoute<View3ViewModel>(
        QStringLiteral("View3"),
        QUrl(QStringLiteral("qrc:/qt/qml/App/UiKit/views/View3.qml")));

    engine.loadFromModule("App.Integration", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[main] Failed to load QML root — aborting";
        return -1;
    }

    return app.exec();
}
