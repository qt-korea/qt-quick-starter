import QtQuick
import QtQuick.Controls

import "../forms"

View1Form {
    id: root

    required property var viewModel

    StackView.onActivated: {
        console.log("[View1] entered | value =", viewModel.value,
                    "| status =", viewModel.status)
        viewModel.notifyPageEntered("View1")
    }

    statusText: "value: " + viewModel.value + "  |  " + viewModel.status
    goView2Btn.onClicked: root.StackView.view.push(Qt.resolvedUrl("View2.qml"),
                                                   { viewModel: root.viewModel })
}
