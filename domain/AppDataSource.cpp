// =============================================================================
//  domain/AppDataSource.cpp
// =============================================================================

#include "AppDataSource.h"
#include <QDebug>

AppDataSource::AppDataSource(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(1000);
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
