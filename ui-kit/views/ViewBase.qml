import QtQuick
import QtQuick.Controls

// =============================================================================
//  ViewBase.qml
//
//  Base type for every page-level View. Wires the QML/StackView page
//  lifecycle to the six virtual hooks on ViewModelBase so concrete views
//  (View1, View2, ...) do not have to repeat the boilerplate.
//
//  Usage:
//
//      // ui-kit/views/View1.qml
//      ViewBase {
//          id: root
//
//          View1Form {
//              anchors.fill: parent
//
//              statusText: "value: " + root.viewModel.value + ...
//              goView2Btn.onClicked: root.StackView.view.pushRoute("View2")
//          }
//      }
//
//  The ViewModel is injected by AppShell when this View is pushed onto
//  the StackView via `StackView.push(url, { viewModel })`. The required
//  property here is what makes that injection contractual — pushing
//  without a viewModel will fail loudly at load time.
//
//  Why an Item, not a Page?
//  ───────────────────────
//  Each ViewNForm is already a `Page` (with its own ColumnLayout etc).
//  Wrapping in another Page would double the framing. An Item is the
//  lightest visual container that still receives StackView attached
//  signals correctly.
//
//  Why six wirings, not fewer?
//  ──────────────────────────
//  The six wirings mirror the six hooks on ViewModelBase. Each hook has
//  an empty default in C++, so a concrete ViewModel that only cares about
//  one of them (say onInitialize) pays no runtime cost for the others —
//  the QML signal dispatches to a no-op virtual call.
//
//  ui-kit purity
//  ─────────────
//  This file imports only standard Qt modules. It is loadable in the
//  `qml` runtime and in Qt Design Studio without the project's C++
//  build, exactly like every other file under ui-kit/.
// =============================================================================

Item {
    id: root

    // Injected by AppShell.pushRoute() → StackView.push(..., { viewModel })
    // when this View is pushed onto the stack.
    required property var viewModel

    readonly property string statusSummary:
        "value: "    + viewModel.value
        + "  |  "    + viewModel.status
        + "  |  enters: " + viewModel.enterCount

    // ─── Lifecycle wiring ─────────────────────────────────────────────────
    Component.onCompleted:    viewModel.onInitialize()
    Component.onDestruction:  viewModel.onFinalize()
    StackView.onActivating:   viewModel.onActivating()
    StackView.onActivated:    viewModel.onActivated()
    StackView.onDeactivating: viewModel.onDeactivating()
    StackView.onDeactivated:  viewModel.onDeactivated()
}
