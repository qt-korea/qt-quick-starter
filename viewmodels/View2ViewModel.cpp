// =============================================================================
//  viewmodels/View2ViewModel.cpp
// =============================================================================

#include "View2ViewModel.h"
#include "AppDataSource.h"

#include <QDebug>

View2ViewModel::View2ViewModel(const AppServices &services, QObject *parent)
    : ViewModelBase(parent)
    , m_services(services)
{
    setObjectName(QStringLiteral("View2VM"));

    if (auto *src = m_services.dataSource) {
        connect(src,  &AppDataSource::valueChanged,
                this, &View2ViewModel::valueChanged);
        connect(src,  &AppDataSource::statusChanged,
                this, &View2ViewModel::statusChanged);
    }
    qInfo().noquote() << "[" + objectName() + "] constructed";
}

int View2ViewModel::value() const
{
    return m_services.dataSource ? m_services.dataSource->value() : 0;
}

QString View2ViewModel::status() const
{
    return m_services.dataSource ? m_services.dataSource->status()
                                 : QStringLiteral("idle");
}

void View2ViewModel::onActivated()
{
    ViewModelBase::onActivated();
    ++m_enterCount;
    emit enterCountChanged();
    qInfo().noquote() << "[" + objectName() + "]"
                      << "  ↳ enter #" << m_enterCount;
}
