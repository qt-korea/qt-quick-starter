// =============================================================================
//  viewmodels/View3ViewModel.h
// =============================================================================

#pragma once

#include "AppServices.h"
#include "ViewModelBase.h"

class View3ViewModel : public ViewModelBase
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by ViewNavigator")

    Q_PROPERTY(int     value      READ value      NOTIFY valueChanged)
    Q_PROPERTY(QString status     READ status     NOTIFY statusChanged)
    Q_PROPERTY(int     enterCount READ enterCount NOTIFY enterCountChanged)

public:
    explicit View3ViewModel(const AppServices &services,
                            QObject           *parent = nullptr);

    QString pageName() const override { return QStringLiteral("View3"); }

    int     value()      const;
    QString status()     const;
    int     enterCount() const { return m_enterCount; }

    void onActivated() override;

signals:
    void valueChanged();
    void statusChanged();
    void enterCountChanged();

private:
    AppServices m_services;
    int         m_enterCount = 0;
};
