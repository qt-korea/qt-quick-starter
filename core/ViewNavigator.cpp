// =============================================================================
//  core/ViewNavigator.cpp
// =============================================================================

#include "ViewNavigator.h"

#include <QDebug>
#include <QQmlEngine>

ViewNavigator::ViewNavigator(QObject *parent)
    : QObject(parent)
{
}

void ViewNavigator::setServices(const AppServices &services)
{
    m_services = services;
}

QVariantMap ViewNavigator::resolveRoute(const QString &name)
{
    const auto it = m_routes.constFind(name);
    if (it == m_routes.constEnd()) {
        qWarning().nospace() << "[ViewNavigator] Unknown route: " << name;
        return {};
    }

    auto *vm = it->factory();
    if (!vm) {
        qWarning().nospace() << "[ViewNavigator] Factory returned null for route: " << name;
        return {};
    }

    // Hand ownership to the QML engine — destroyed when no QML reference holds it.
    QQmlEngine::setObjectOwnership(vm, QQmlEngine::JavaScriptOwnership);

    return QVariantMap{
        { QStringLiteral("url"),       it->qmlUrl },
        { QStringLiteral("viewModel"), QVariant::fromValue<QObject *>(vm) },
    };
}
