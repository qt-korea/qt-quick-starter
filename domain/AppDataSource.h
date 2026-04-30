// =============================================================================
//  domain/AppDataSource.h
//
//  Sample domain object — holds an int and a QString, auto-updated every
//  second via QTimer. Demonstrates the pattern: a C++ class performs logic,
//  emits signals, and a ViewModel listens and forwards to QML.
//
//  In a real app, replace the timer with I/O: a sensor read, a network
//  response, a database query, etc. The ViewModel binding stays identical.
//
//  No QML/Quick dependency. Unit-testable with plain QtTest.
// =============================================================================

#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

// Not thread-safe — must remain on the GUI thread (QTimer constraint).
class AppDataSource : public QObject
{
    Q_OBJECT
public:
    explicit AppDataSource(QObject *parent = nullptr);

    int     value()  const { return m_value; }
    QString status() const { return m_status; }

public slots:
    void onTick();

signals:
    void valueChanged(int value);
    void statusChanged(const QString &status);

private:
    QTimer  *m_timer;
    int      m_value  = 0;
    QString  m_status = QStringLiteral("idle");
};
