import QtQuick
import QtQuick.Controls

import "../forms"

ViewBase {
    id: root

    View1Form {
        anchors.fill: parent

        statusText: root.statusSummary

        goView2Btn.onClicked: if (root.StackView.view) root.StackView.view.pushRoute("View2")
    }
}
