import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Page {
    id: root

    width: 480
    height: 800

    property alias statusText: statusLabel.text
    property alias backBtn:    backBtn

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        Label {
            text: "View 3"
            font.pixelSize: 32
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            id: statusLabel
            text: "value: 0  |  idle"
            Layout.alignment: Qt.AlignHCenter
        }

        AppButton {
            id: backBtn
            text: "‹ Back to View 2"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
