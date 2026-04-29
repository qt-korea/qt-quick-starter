// =============================================================================
//  core/ViewNavigator.h
//
//  Single source of truth for route definitions. A "route" is a name
//  (string) that maps to:
//    1. A QML view URL  (a .qml file from ui-kit/views/)
//    2. A ViewModel factory  (a function that builds a ViewModelBase*)
//
//  Lives in core/ alongside the other reusable infrastructure types
//  (ViewModelBase, AppServices). The ViewNavigator is what turns the
//  route name passed by AppShell.pushRoute() into a fresh View+ViewModel
//  pair.
//
//  Registered as QML_SINGLETON in the App.Integration module so QML can
//  call ViewNavigator.resolveRoute(name) from AppShell to discover what
//  to push.
//
//  ─── Why "ViewNavigator", not "Router" ────────────────────────────────
//
//  This class navigates between QML *views* (and their matching
//  ViewModels). "Router" is too generic — the user might write their
//  own routing class for HTTP, IPC, or message dispatch. Naming it
//  ViewNavigator keeps the role explicit and avoids the name clash.
//
//  ─── Ownership model ──────────────────────────────────────────────────
//
//  Each call to resolveRoute() constructs a fresh ViewModel instance.
//  Ownership is handed to the QML JavaScript engine via
//  setObjectOwnership(JavaScriptOwnership) — the ViewModel is destroyed
//  when no QML reference remains, i.e. shortly after the View is popped.
//  No singletons, no caching, no manual cleanup.
//
//  ─── Service wiring ───────────────────────────────────────────────────
//
//  setServices() is called once at startup with the AppServices bundle
//  (see core/AppServices.h). Every ViewModel created by the navigator
//  receives that same bundle by const reference and stores its own copy.
//  Adding a new service to the bundle does NOT change this file — that
//  is the whole point of routing services through a struct rather than
//  through individual setter calls.
// =============================================================================

#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariantMap>
#include <QtQmlIntegration/qqmlintegration.h>

#include <functional>
#include <type_traits>

#include "AppServices.h"
#include "ViewModelBase.h"

class ViewNavigator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit ViewNavigator(QObject *parent = nullptr);

    // Called once from main.cpp before any route resolution.
    // The bundle is copied by value; the pointed-to services must
    // outlive every ViewModel the navigator will create (typically they
    // live for the entire application lifetime).
    void setServices(const AppServices &services);

    // Register a route. The ViewModel type must derive from
    // ViewModelBase and accept (const AppServices&, QObject*) in its
    // constructor. This factory does NOT need to change when new
    // services are added to AppServices.
    template<typename TViewModel>
    void registerRoute(const QString &name, const QUrl &qmlUrl)
    {
        static_assert(std::is_base_of_v<ViewModelBase, TViewModel>,
                      "TViewModel must derive from ViewModelBase");
        Route route;
        route.qmlUrl = qmlUrl;
        route.factory = [this]() -> ViewModelBase *
        {
            return new TViewModel(m_services, nullptr);
        };
        m_routes.insert(name, route);
    }

    // Called from QML (AppShell.qml). Returns:
    //   { "url": <QUrl>, "viewModel": <ViewModelBase*> }
    // or an empty map if the name is unknown.
    Q_INVOKABLE QVariantMap resolveRoute(const QString &name);

private:
    struct Route
    {
        QUrl qmlUrl;
        std::function<ViewModelBase *()> factory;
    };

    QHash<QString, Route> m_routes;
    AppServices           m_services;
};
