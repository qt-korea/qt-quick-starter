import QtQuick
import QtQuick.Controls

import "../forms"

View2Form {
    id: root

    required property var viewModel

    StackView.onActivated: {
        console.log("[View2] entered | value =", viewModel.value,
                    "| lastVisitedPage =", viewModel.lastVisitedPage)
        viewModel.notifyPageEntered("View2")
    }

    statusText:           "value: " + viewModel.value + "  |  " + viewModel.status
    backBtn.onClicked:    root.StackView.view.pop()
    goView3Btn.onClicked: root.StackView.view.push(Qt.resolvedUrl("View3.qml"),
                                                    { viewModel: root.viewModel })
}
