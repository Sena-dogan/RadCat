import QtQuick
import QtQuick.Controls

Button {
    id: control

    property string buttonText: "Scan for Devices"
    property color normalColor: "#2563eb"
    property color hoverColor: "#3b82f6"
    property color pressedColor: "#1040a0"

    text: ""

    background: Rectangle {
        color: if (control.down) {
            return control.pressedColor;
        } else if (control.hovered) {
            return control.hoverColor;
        } else {
            return control.normalColor;
        }

        radius: 8
        implicitWidth: 220
        implicitHeight: 50
        border.color: Qt.lighter(control.normalColor, 1.2)
        border.width: 1

        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
    }

    contentItem: Item {
        anchors.fill: parent

        Row {
            spacing: 10
            anchors.centerIn: parent

            Rectangle {
                width: 24
                height: 24
                radius: 12
                color: "transparent"
                border.color: "white"
                border.width: 2

                Text {
                    text: "Q"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 14
                    anchors.centerIn: parent
                }
            }

            Text {
                text: control.buttonText
                color: "white"
                font.bold: true
                font.pixelSize: 16
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
