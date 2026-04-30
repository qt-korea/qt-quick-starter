// =============================================================================
//  viewmodels/View3ViewModel.cpp
// =============================================================================

#include "View3ViewModel.h"
#include "AppDataSource.h"

#include <QDebug>

View3ViewModel::View3ViewModel(const AppServices &services, QObject *parent)
    : ViewModelBase(parent)
    , m_services(services)
{
    setObjectName(QStringLiteral("View3VM"));

    if (auto *src = m_services.dataSource) {
        connect(src,  &AppDataSource::valueChanged,
                this, &View3ViewModel::valueChanged);
        connect(src,  &AppDataSource::statusChanged,
                this, &View3ViewModel::statusChanged);
    }
    qInfo().noquote() << '[' << objectName() << "] constructed";
}

int View3ViewModel::value() const
{
    return m_services.dataSource ? m_services.dataSource->value() : 0;
}

QString View3ViewModel::status() const
{
    return m_services.dataSource ? m_services.dataSource->status()
                                 : QStringLiteral("idle");
}

void View3ViewModel::onActivated()
{
    ViewModelBase::onActivated();
    ++m_enterCount;
    emit enterCountChanged();
    qInfo().noquote() << '[' << objectName() << ']'
                      << "  ↳ enter #" << m_enterCount;
}
