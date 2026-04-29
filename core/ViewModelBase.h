// =============================================================================
//  core/ViewModelBase.h
//
//  Abstract base class for page-scoped ViewModels.
//
//  Each route has exactly one ViewModel class derived from ViewModelBase.
//  The ViewNavigator (in core/) is responsible for instantiating one of
//  these per push() and for handing it to the View as a `viewModel`
//  property.
//
//  ─── Lifecycle hooks ────────────────────────────────────────────────────
//
//  The View wires these to QML/StackView attached signals (see
//  ui-kit/views/ViewBase.qml). The base class supplies a default log
//  message for every hook so the developer can immediately see the
//  sequence in the console while clicking through the demo. Override
//  in derived classes when you have real work to do — and call the
//  base hook from your override if you want to keep the diagnostic log:
//
//      void View1ViewModel::onActivated() {
//          ViewModelBase::onActivated();      // ← keep the log line
//          ++m_enterCount;                    // ← page-specific work
//          emit enterCountChanged();
//      }
//
//  The base uses QObject::objectName() to label its log lines, so each
//  derived class only has to call setObjectName(...) once in its
//  constructor and the entire log set is automatically named for it.
//
//  Typical sequence for a page that is pushed, covered by another, then
//  uncovered, then popped:
//
//      Navigator constructs the ViewModel      ◄ C++ constructor
//          ↓                                    (lightweight plumbing only)
//      View QML object built, viewModel set
//          ↓
//      onInitialize()                         ◄ ONE-SHOT — heavy data load,
//          ↓                                    async fetches, big alloc
//      onActivating() → onActivated()         ◄ page becomes visible
//          ↓                                    (start timers, sensors,
//                                                animations)
//      onDeactivating() → onDeactivated()     ◄ another page pushed on top
//          ↓                                    (pause subscriptions, save
//                                                scroll position)
//      ... ViewModel stays alive while inactive ...
//          ↓
//      onActivating() → onActivated()         ◄ top page popped, this one
//          ↓                                    is visible again — note
//                                                onInitialize is NOT
//                                                called twice
//      onDeactivating() → onDeactivated()     ◄ this page is being popped
//          ↓
//      onFinalize()                           ◄ Component.onDestruction —
//          ↓                                    last chance to release
//                                                resources while view +
//                                                viewmodel both still exist
//      ViewModel destroyed by JS GC
//
//  ─── Why default log bodies, not pure virtual ─────────────────────────
//
//  Most pages only override two or three hooks. Empty defaults would
//  leave silent gaps in the demo's console output; pure virtual would
//  force every subclass to write empty bodies for hooks it does not
//  care about. A default log line gives the new developer a free
//  diagnostic and is trivially overridden when real behaviour is
//  needed.
//
//  ─── Threading ──────────────────────────────────────────────────────────
//
//  All hooks are called from the GUI thread (StackView signals + QML
//  Component lifecycle are GUI-thread). Safe to touch QObject members
//  directly. Long-running work should be dispatched off-thread by the
//  override itself.
//
//  ─── Naming convention ─────────────────────────────────────────────────
//
//  This template follows the Qt convention of avoiding C++ namespaces.
//  Type names are unique on their own (`ViewModelBase`, `AppServices`,
//  `ViewNavigator`, `AppDataSource`, `View1ViewModel`, …). The folder
//  boundary `core/` — not a namespace — marks the infrastructure /
//  demo line.
// =============================================================================

#pragma once

#include <QObject>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class ViewModelBase : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("ViewModelBase is abstract — instances are created by the ViewNavigator.")

    Q_PROPERTY(QString pageName READ pageName CONSTANT)

public:
    explicit ViewModelBase(QObject *parent = nullptr) : QObject(parent) {}
    ~ViewModelBase() override = default;

    virtual QString pageName() const = 0;

    // ─── Lifecycle hooks ────────────────────────────────────────────────
    //
    //  Each hook ships with a default log line that uses objectName() as
    //  its label. Derived classes override when they have real work; if
    //  they want to keep the log, they call the base implementation
    //  first (see header comment for the override pattern).
    //
    Q_INVOKABLE virtual void onInitialize();
    Q_INVOKABLE virtual void onActivating();
    Q_INVOKABLE virtual void onActivated();
    Q_INVOKABLE virtual void onDeactivating();
    Q_INVOKABLE virtual void onDeactivated();
    Q_INVOKABLE virtual void onFinalize();
};
