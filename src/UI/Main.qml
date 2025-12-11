import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 900
    height: 600
    title: "LabControl"
    color: "#0f111a"

    menuBar: AppMenuBar {}

    ColumnLayout {
        spacing: 25
        anchors.centerIn: parent

        Text {
            text: "Workspace is Empty"
            color: "white"
            font.pixelSize: 32
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: "Connect a device or scan the network to begin."
            color: "#8b949e"
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        Item {
            width: 1
            height: 15
        }

        ScanButton {
            buttonText: "Scan for Devices"
            Layout.alignment: Qt.AlignHCenter
            onClicked: console.log("Scanning started...")
        }
    }
}
