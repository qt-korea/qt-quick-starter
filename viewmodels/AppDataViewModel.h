// =============================================================================
//  viewmodels/AppDataViewModel.h
//
//  The single ViewModel for the demo app. Registered as a QML_SINGLETON
//  in the App.ViewModels module so all views can bind to the same instance
//  without explicit injection. Holds UI-shaped state and forwards domain
//  signals from AppDataSource.
//
//  In small projects this single-singleton pattern is the simplest thing
//  that works. As the page count grows, consider the per-page ViewModel
//  pattern on the `feature/routed-viewmodels` branch instead.
// =============================================================================

#pragma once

#include <QObject>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class AppDataSource;

class AppDataViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int value READ value NOTIFY valueChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lastVisitedPage READ lastVisitedPage NOTIFY lastVisitedPageChanged)

public:
    explicit AppDataViewModel(QObject *parent = nullptr);

    void bindDataSource(AppDataSource *source);

    int     value()           const;
    QString status()          const;
    QString lastVisitedPage() const { return m_lastVisitedPage; }

    Q_INVOKABLE void notifyPageEntered(const QString &pageName);

signals:
    void valueChanged(int value);
    void statusChanged(const QString &status);
    void lastVisitedPageChanged(const QString &pageName);

private:
    AppDataSource *m_source = nullptr;
    QString        m_lastVisitedPage;
};
