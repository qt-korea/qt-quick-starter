import QtQuick
import QtTest
import App.UiKit

Item {
    width: 480; height: 800

    View1Form { id: view1 }
    View2Form { id: view2 }
    View3Form { id: view3 }

    TestCase {
        name: "View1FormLayout"
        when: windowShown

        function test_default_status() {
            compare(view1.statusText, "value: 0  |  idle")
        }
        function test_button_exists() {
            verify(view1.goView2Btn !== null)
        }
    }

    TestCase {
        name: "View2FormLayout"
        when: windowShown

        function test_default_status() {
            compare(view2.statusText, "value: 0  |  idle")
        }
        function test_buttons_exist() {
            verify(view2.backBtn !== null)
            verify(view2.goView3Btn !== null)
        }
    }

    TestCase {
        name: "View3FormLayout"
        when: windowShown

        function test_default_status() {
            compare(view3.statusText, "value: 0  |  idle")
        }
        function test_button_exists() {
            verify(view3.backBtn !== null)
        }
    }
}
