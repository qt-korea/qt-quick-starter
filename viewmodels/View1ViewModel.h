// =============================================================================
//  viewmodels/View1ViewModel.h
//
//  ViewModel scoped to the View1 page. One instance per push(); destroyed
//  with the View when the page is popped.
//
//  Only the hooks that have real page-specific behaviour are overridden
//  here (onActivated — to bump enterCount). The other lifecycle hooks
//  use the default log-only implementation supplied by ViewModelBase,
//  labelled by objectName(). Replace these defaults with real work
//  (data fetch, timer start, resource release) when you grow this page
//  into a real one.
//
//  Service access: the constructor takes the AppServices bundle by const
//  reference and stores its own copy. Add a new service to the bundle in
//  core/AppServices.h and this VM can immediately read it without any
//  signature change.
// =============================================================================

#pragma once

#include "AppServices.h"
#include "ViewModelBase.h"

class View1ViewModel : public ViewModelBase
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by ViewNavigator")

    Q_PROPERTY(int     value      READ value      NOTIFY valueChanged)
    Q_PROPERTY(QString status     READ status     NOTIFY statusChanged)
    Q_PROPERTY(int     enterCount READ enterCount NOTIFY enterCountChanged)

public:
    explicit View1ViewModel(const AppServices &services,
                            QObject           *parent = nullptr);

    QString pageName() const override { return QStringLiteral("View1"); }

    int     value()      const;
    QString status()     const;
    int     enterCount() const { return m_enterCount; }

    // Page-specific override. Calls the base first to keep the diagnostic
    // log, then performs page-local work.
    void onActivated() override;

signals:
    void valueChanged();
    void statusChanged();
    void enterCountChanged();

private:
    AppServices m_services;
    int         m_enterCount = 0;
};
