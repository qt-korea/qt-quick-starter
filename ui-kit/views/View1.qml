import QtQuick
import QtQuick.Controls
import App.ViewModels

import "../forms"

View1Form {
    id: root

    required property AppDataViewModel viewModel

    readonly property url view2Url: Qt.resolvedUrl("View2.qml")

    StackView.onActivated: {
        console.log("[View1] entered | value =", root.viewModel.value,
                    "| status =", root.viewModel.status)
        root.viewModel.notifyPageEntered("View1")
    }

    statusText: "value: " + root.viewModel.value + "  |  " + root.viewModel.status
    goView2Btn.onClicked: {
        if (root.StackView.view)
            root.StackView.view.push(root.view2Url, { viewModel: root.viewModel })
    }
}
