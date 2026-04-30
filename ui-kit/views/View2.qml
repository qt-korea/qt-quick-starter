import QtQuick
import QtQuick.Controls
import App.ViewModels

import "../forms"

View2Form {
    id: root

    required property AppDataViewModel viewModel

    readonly property url view3Url: Qt.resolvedUrl("View3.qml")

    StackView.onActivated: {
        console.log("[View2] entered | value =", root.viewModel.value,
                    "| lastVisitedPage =", root.viewModel.lastVisitedPage)
        root.viewModel.notifyPageEntered("View2")
    }

    statusText:           "value: " + root.viewModel.value + "  |  " + root.viewModel.status
    backBtn.onClicked:    { if (root.StackView.view) root.StackView.view.pop() }
    goView3Btn.onClicked: {
        if (root.StackView.view)
            root.StackView.view.push(root.view3Url, { viewModel: root.viewModel })
    }
}
