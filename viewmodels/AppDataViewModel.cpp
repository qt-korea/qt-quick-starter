// =============================================================================
//  viewmodels/AppDataViewModel.cpp
//
//  The constructor sets the object name once. All log lines below use
//  objectName() instead of a hardcoded string, so renaming the class only
//  requires updating the setObjectName() call.
// =============================================================================

#include "AppDataViewModel.h"
#include "AppDataSource.h"

#include <QDebug>

AppDataViewModel::AppDataViewModel(QObject *parent)
    : QObject(parent)
{
    setObjectName(QStringLiteral("AppDataViewModel"));
}

void AppDataViewModel::bindDataSource(AppDataSource *source)
{
    if (source == nullptr) return;

    m_source = source;

    connect(m_source, &AppDataSource::valueChanged,
            this,     &AppDataViewModel::valueChanged);
    connect(m_source, &AppDataSource::statusChanged,
            this,     &AppDataViewModel::statusChanged);
}

int AppDataViewModel::value() const
{
    return m_source ? m_source->value() : 0;
}

QString AppDataViewModel::status() const
{
    return m_source ? m_source->status() : QStringLiteral("idle");
}

void AppDataViewModel::notifyPageEntered(const QString &pageName)
{
    m_lastVisitedPage = pageName;
    emit lastVisitedPageChanged(pageName);
    qInfo().noquote().nospace()
        << "[" << objectName() << "] page entered: " << pageName
        << " | value = "  << value()
        << " | status = " << status();
}
