import QtQuick
import QtQuick.Controls
import App.ViewModels

import "../forms"

View3Form {
    id: root

    required property AppDataViewModel viewModel

    StackView.onActivated: {
        console.log("[View3] entered | value =", root.viewModel.value,
                    "| lastVisitedPage =", root.viewModel.lastVisitedPage)
        root.viewModel.notifyPageEntered("View3")
    }

    statusText:        "value: " + root.viewModel.value + "  |  " + root.viewModel.status
    backBtn.onClicked: { if (root.StackView.view) root.StackView.view.pop() }
}
