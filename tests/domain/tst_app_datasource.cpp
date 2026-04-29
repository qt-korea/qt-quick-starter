// =============================================================================
//  tests/domain/tst_app_datasource.cpp
//
//  Plain QtTest for AppDataSource — no display, no QML.
// =============================================================================

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "AppDataSource.h"

class TestAppDataSource : public QObject
{
    Q_OBJECT
private slots:
    void initial_state_is_zero_and_idle();
    void tick_increments_value_and_emits_signal();
    void tick_updates_status_string();
    void multiple_ticks_accumulate();
};

void TestAppDataSource::initial_state_is_zero_and_idle()
{
    AppDataSource src;
    QCOMPARE(src.value(),  0);
    QCOMPARE(src.status(), QStringLiteral("idle"));
}

void TestAppDataSource::tick_increments_value_and_emits_signal()
{
    AppDataSource src;
    QSignalSpy spy(&src, &AppDataSource::valueChanged);

    src.onTick();

    QCOMPARE(src.value(), 1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toInt(), 1);
}

void TestAppDataSource::tick_updates_status_string()
{
    AppDataSource src;
    QSignalSpy spy(&src, &AppDataSource::statusChanged);

    src.onTick();

    QCOMPARE(src.status(), QStringLiteral("tick #1"));
    QCOMPARE(spy.count(), 1);
}

void TestAppDataSource::multiple_ticks_accumulate()
{
    AppDataSource src;

    src.onTick();
    src.onTick();
    src.onTick();

    QCOMPARE(src.value(),  3);
    QCOMPARE(src.status(), QStringLiteral("tick #3"));
}

QTEST_GUILESS_MAIN(TestAppDataSource)
#include "tst_app_datasource.moc"
