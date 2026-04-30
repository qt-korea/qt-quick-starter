import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Page {
    id: root

    property alias statusText: statusLabel.text
    property alias goView2Btn: goView2Btn

    width: 480
    height: 800

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        Label {
            text:       "View 1"
            font.pixelSize: 32
            font.bold:  true
            textFormat: Text.PlainText
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            id:         statusLabel
            text:       "value: 0  |  idle"
            textFormat: Text.PlainText
            Layout.alignment: Qt.AlignHCenter
        }

        AppButton {
            id: goView2Btn
            text: "Go to View 2"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
