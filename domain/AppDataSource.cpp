// =============================================================================
//  domain/AppDataSource.cpp
// =============================================================================

#include "AppDataSource.h"
#include <QDebug>

static constexpr int kTickIntervalMs = 1000;

AppDataSource::AppDataSource(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))  // parented to this, destroyed automatically
{
    m_timer->setInterval(kTickIntervalMs);
    connect(m_timer, &QTimer::timeout, this, &AppDataSource::onTick);
    m_timer->start();
}

void AppDataSource::onTick()
{
    ++m_value;
    emit valueChanged(m_value);

    m_status = QStringLiteral("tick #%1").arg(m_value);
    emit statusChanged(m_status);
}
