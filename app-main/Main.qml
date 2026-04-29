import QtQuick

import App.UiKit
import App.ViewModels

Window {
    width: 480
    height: 800
    visible: true
    title: "Qt Quick Starter"

    AppShell {
        anchors.fill: parent
        viewModel: AppDataViewModel
    }
}
