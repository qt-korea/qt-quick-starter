import QtQuick

import App.UiKit
import App.Core

Window {
    width: 480
    height: 800
    visible: true
    title: "Qt Quick Starter"

    AppShell {
        anchors.fill: parent
        navigator: ViewNavigator     // QML_SINGLETON from App.Core
        initialRoute: "View1"
    }
}
