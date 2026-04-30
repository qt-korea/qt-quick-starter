import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Page {
    id: root

    property alias statusText: statusLabel.text
    property alias backBtn:    backBtn
    property alias goView3Btn: goView3Btn

    width: 480
    height: 800

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        Label {
            text:       "View 2"
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

        RowLayout {
            spacing: 12
            Layout.alignment: Qt.AlignHCenter

            AppButton {
                id: backBtn
                text: "‹ Back to View 1"
            }

            AppButton {
                id: goView3Btn
                text: "Go to View 3"
            }
        }
    }
}
