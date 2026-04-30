pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import App.ViewModels

import "views"

Item {
    id: root

    required property AppDataViewModel viewModel

    StackView {
        anchors.fill: parent
        initialItem: Component {
            View1 { viewModel: root.viewModel }
        }
    }
}
