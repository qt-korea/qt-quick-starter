import QtQuick
import QtQuick.Templates as T

T.Button {
    id: control

    implicitWidth:  Math.max(implicitBackgroundWidth  + leftInset  + rightInset,
                             implicitContentWidth     + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset   + bottomInset,
                             implicitContentHeight    + topPadding + bottomPadding)

    padding: 10
    horizontalPadding: 20

    contentItem: Text {
        text:                control.text
        font:                control.font
        color:               control.enabled ? "#ffffff" : "#80ffffff"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:   Text.AlignVCenter
        elide:               Text.ElideRight
    }

    background: Rectangle {
        implicitWidth:  120
        implicitHeight: 40
        radius:         6
        color: control.down    ? "#1565c0"
             : control.hovered ? "#1976d2"
             :                   "#2196f3"

        Behavior on color { ColorAnimation { duration: 150 } }
    }
}
