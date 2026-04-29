// =============================================================================
//  viewmodels/View1ViewModel.cpp
//
//  Constructor sets the object name (used by ViewModelBase's diagnostic
//  log lines) and wires the relevant domain signals. The rest of the
//  page lifecycle is handled by ViewModelBase's default hooks; this VM
//  only overrides onActivated() because it has page-local state
//  (enterCount) to update.
// =============================================================================

#include "View1ViewModel.h"
#include "AppDataSource.h"

#include <QDebug>

View1ViewModel::View1ViewModel(const AppServices &services, QObject *parent)
    : ViewModelBase(parent)
    , m_services(services)
{
    setObjectName(QStringLiteral("View1VM"));

    // Constructor: cheap plumbing only. No I/O, no allocations.
    // Heavy work belongs in onInitialize().
    if (auto *src = m_services.dataSource) {
        connect(src,  &AppDataSource::valueChanged,
                this, &View1ViewModel::valueChanged);
        connect(src,  &AppDataSource::statusChanged,
                this, &View1ViewModel::statusChanged);
    }
    qInfo().noquote() << "[" + objectName() + "] constructed";
}

int View1ViewModel::value() const
{
    return m_services.dataSource ? m_services.dataSource->value() : 0;
}

QString View1ViewModel::status() const
{
    return m_services.dataSource ? m_services.dataSource->status()
                                 : QStringLiteral("idle");
}

void View1ViewModel::onActivated()
{
    ViewModelBase::onActivated();   // keep the diagnostic log line
    ++m_enterCount;
    emit enterCountChanged();
    qInfo().noquote() << "[" + objectName() + "]"
                      << "  ↳ enter #" << m_enterCount;
}
