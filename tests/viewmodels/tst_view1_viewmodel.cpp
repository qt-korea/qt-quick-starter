// =============================================================================
//  tests/viewmodels/tst_view1_viewmodel.cpp
//
//  Unit tests for View1ViewModel.
//
//  Why this file exists
//  ────────────────────
//  ARCHITECTURE.md → "Wiring domain services" claims that the AppServices
//  pattern lets each test construct only the services it needs, with no
//  globals to reset between tests. This file is the running proof of that
//  claim — if AppServices ever stops being testable that way, this file
//  fails to compile or fails at runtime, and the documentation lies.
//
//  Why only View1
//  ──────────────
//  View2ViewModel and View3ViewModel are the same shape as View1ViewModel
//  by design (they share the per-page-VM pattern from ViewModelBase).
//  Testing one is enough to lock the pattern; copy-pasting the same five
//  test cases for two near-identical classes adds maintenance cost without
//  catching new bugs. If the three diverge in the future, add focused
//  tests for the diverging behaviour only.
//
//  No display
//  ──────────
//  Uses QTEST_GUILESS_MAIN — no QGuiApplication, no QML engine, no window.
//  The ViewModel does not depend on either; lifecycle hooks are plain
//  C++ virtual calls when invoked directly from the test (a real app
//  invokes them via QML/StackView signals, but the C++ contract is the
//  same).
// =============================================================================

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "AppDataSource.h"
#include "AppServices.h"
#include "View1ViewModel.h"

class TestView1ViewModel : public QObject
{
    Q_OBJECT
private slots:
    void default_state_with_empty_services();
    void reflects_data_source_value();
    void emits_value_changed_when_data_source_ticks();
    void enter_count_increments_on_activation();
    void independent_instances_have_independent_enter_counts();
};

// ─── Tests ────────────────────────────────────────────────────────────────

// An empty AppServices bundle (all members nullptr) must still produce a
// valid VM — no crashes, sensible defaults. This is what makes
// "construct only what you need" possible in every other test below.
void TestView1ViewModel::default_state_with_empty_services()
{
    AppServices    services;          // every member nullptr
    View1ViewModel vm(services);

    QCOMPARE(vm.value(),      0);
    QCOMPARE(vm.status(),     QStringLiteral("idle"));
    QCOMPARE(vm.enterCount(), 0);
    QCOMPARE(vm.pageName(),   QStringLiteral("View1"));
}

// With dataSource wired, the VM proxies the domain state.
void TestView1ViewModel::reflects_data_source_value()
{
    AppDataSource src;
    AppServices   services;
    services.dataSource = &src;

    View1ViewModel vm(services);

    src.onTick();
    src.onTick();

    QCOMPARE(vm.value(),  2);
    QCOMPARE(vm.status(), QStringLiteral("tick #2"));
}

// AppDataSource::valueChanged must propagate to View1ViewModel::valueChanged.
// This is the actual signal contract Views bind to, so it earns its own test.
void TestView1ViewModel::emits_value_changed_when_data_source_ticks()
{
    AppDataSource src;
    AppServices   services;
    services.dataSource = &src;

    View1ViewModel vm(services);
    QSignalSpy spy(&vm, &View1ViewModel::valueChanged);

    src.onTick();

    QCOMPARE(spy.count(), 1);
}

// onActivated() is the lifecycle hook that increments enterCount and
// emits enterCountChanged. The full hook set (onInitialize/onFinalize/etc)
// is plain virtual dispatch — exercising one representative hook proves
// the wiring without re-testing every empty default.
void TestView1ViewModel::enter_count_increments_on_activation()
{
    AppServices    services;
    View1ViewModel vm(services);
    QSignalSpy spy(&vm, &View1ViewModel::enterCountChanged);

    vm.onActivated();
    vm.onActivated();
    vm.onActivated();

    QCOMPARE(vm.enterCount(), 3);
    QCOMPARE(spy.count(),     3);
}

// Two VMs sharing the same services bundle must keep their page-scoped
// state (enterCount) independent. This is the central guarantee of
// "page-scoped instance, shared domain" — if it ever breaks, the per-page
// VM pattern silently regresses to shared state.
void TestView1ViewModel::independent_instances_have_independent_enter_counts()
{
    AppDataSource src;
    AppServices   services;
    services.dataSource = &src;

    View1ViewModel vmA(services);
    View1ViewModel vmB(services);

    vmA.onActivated();
    vmA.onActivated();
    vmB.onActivated();

    QCOMPARE(vmA.enterCount(), 2);
    QCOMPARE(vmB.enterCount(), 1);

    // Sanity: domain state is shared (same source, both VMs see the same value).
    src.onTick();
    QCOMPARE(vmA.value(), 1);
    QCOMPARE(vmB.value(), 1);
}

QTEST_GUILESS_MAIN(TestView1ViewModel)
#include "tst_view1_viewmodel.moc"
