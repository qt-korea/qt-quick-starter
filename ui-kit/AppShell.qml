pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import "views"

Item {
    id: root

    required property var viewModel

    StackView {
        anchors.fill: parent
        initialItem: Component {
            View1 { viewModel: root.viewModel }
        }
    }
}
