import QtQuick
import QtQuick.Controls

import "../forms"

ViewBase {
    id: root

    View2Form {
        anchors.fill: parent

        statusText: root.statusSummary

        backBtn.onClicked:    if (root.StackView.view) root.StackView.view.pop()
        goView3Btn.onClicked: if (root.StackView.view) root.StackView.view.pushRoute("View3")
    }
}
