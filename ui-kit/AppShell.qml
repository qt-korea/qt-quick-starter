pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

// AppShell — the navigation host.
//
// Owns the StackView and exposes a `pushRoute(name)` helper. It does NOT
// import any project-owned C++ module. The ViewNavigator is received as a
// duck-typed `required property var navigator`, the same injection pattern
// Views use to receive `viewModel`. This keeps ui-kit/ free of
// project-owned C++ dependencies — the ui-kit purity rule still holds.
//
// Children navigate by calling `StackView.view.pushRoute("View2")`.
// They never see the ViewNavigator type or any ViewModel type directly.

Item {
    id: root

    // Injected from app-main/Main.qml. Must expose:
    //   resolveRoute(name) -> { url, viewModel }
    required property var navigator

    // Optional first route. Defaults to "View1" so the existing demo
    // continues to work, but app-main can override it if it wishes.
    property string initialRoute: "View1"

    StackView {
        id: stack
        anchors.fill: parent

        // Build a page from the route table and push it. The View receives
        // the freshly-built ViewModel via `initialProperties`.
        function pushRoute(name) {
            const r = root.navigator.resolveRoute(name)
            if (!r || !r.url || !r.viewModel) {
                console.warn("[AppShell] Unknown route:", name)
                return null
            }
            const item = push(r.url, { viewModel: r.viewModel })
            if (!item) console.error("[AppShell] push failed for route:", name)
            return item
        }

        Component.onCompleted: pushRoute(root.initialRoute)
    }
}
