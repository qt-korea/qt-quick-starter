// =============================================================================
//  core/ViewModelBase.cpp
//
//  Default implementations for the six lifecycle hooks. Each prints a
//  diagnostic line tagged with objectName() so the developer can watch
//  the page lifecycle in the console while clicking through the demo.
//
//  Derived classes that want to keep the diagnostic call the base
//  implementation first; derived classes that do not want it just
//  override and do not call up. There is no "magic" silently happening
//  — every log line is one of these methods.
// =============================================================================

#include "ViewModelBase.h"

#include <QDebug>

void ViewModelBase::onInitialize()
{
    qInfo().noquote() << "[" + objectName() + "]"
                      << "onInitialize  — load initial data, allocate buffers";
}

void ViewModelBase::onActivating()
{
    qInfo().noquote() << "[" + objectName() + "]"
                      << "onActivating  — page transition in";
}

void ViewModelBase::onActivated()
{
    qInfo().noquote() << "[" + objectName() + "]"
                      << "onActivated   — visible";
}

void ViewModelBase::onDeactivating()
{
    qInfo().noquote() << "[" + objectName() + "]"
                      << "onDeactivating— page transition out";
}

void ViewModelBase::onDeactivated()
{
    qInfo().noquote() << "[" + objectName() + "]"
                      << "onDeactivated — hidden (still alive)";
}

void ViewModelBase::onFinalize()
{
    qInfo().noquote() << "[" + objectName() + "]"
                      << "onFinalize    — release resources, persist state";
}
