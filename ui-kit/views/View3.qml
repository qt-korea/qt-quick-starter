import QtQuick
import QtQuick.Controls

import "../forms"

View3Form {
    id: root

    required property var viewModel

    StackView.onActivated: {
        console.log("[View3] entered | value =", viewModel.value,
                    "| lastVisitedPage =", viewModel.lastVisitedPage)
        viewModel.notifyPageEntered("View3")
    }

    statusText:        "value: " + viewModel.value + "  |  " + viewModel.status
    backBtn.onClicked: root.StackView.view.pop()
}
