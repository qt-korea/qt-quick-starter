import QtQuick
import QtQuick.Controls

import "../forms"

ViewBase {
    id: root

    View3Form {
        anchors.fill: parent

        statusText: root.statusSummary

        backBtn.onClicked: if (root.StackView.view) root.StackView.view.pop()
    }
}
