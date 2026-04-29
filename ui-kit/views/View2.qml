import QtQuick
import QtQuick.Controls

import "../forms"

ViewBase {
    id: root

    View2Form {
        anchors.fill: parent

        statusText: "value: " + root.viewModel.value
                  + "  |  " + root.viewModel.status
                  + "  |  enters: " + root.viewModel.enterCount

        backBtn.onClicked:    root.StackView.view.pop()
        goView3Btn.onClicked: root.StackView.view.pushRoute("View3")
    }
}
