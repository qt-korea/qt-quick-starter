import QtQuick

import App.UiKit
import App.ViewModels

Window {
    id: root
    width: 480
    height: 800
    visible: true
    title: "Qt Quick Starter"

    AppShell {
        anchors.fill: parent
        viewModel: AppDataViewModel
    }
}
