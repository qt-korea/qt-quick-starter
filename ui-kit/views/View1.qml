import QtQuick
import QtQuick.Controls

import "../forms"

ViewBase {
    id: root

    View1Form {
        anchors.fill: parent

        statusText: "value: " + root.viewModel.value
                  + "  |  " + root.viewModel.status
                  + "  |  enters: " + root.viewModel.enterCount

        goView2Btn.onClicked: root.StackView.view.pushRoute("View2")
    }
}
