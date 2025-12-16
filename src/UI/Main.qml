import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RadCat

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 900
    height: 600
    title: "LabControl"
    color: "#0f111a"

    menuBar: AppMenuBar {}

    // State to toggle between Empty View and List View
    property bool isScanningMode: false

    // --- 1. EMPTY STATE VIEW ---
    ColumnLayout {
        // Explicitly accessing 'mainWindow.isScanningMode' to avoid lint warnings
        visible: !mainWindow.isScanningMode
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
            onClicked: {
                console.log("Switching to list view.");
                mainWindow.isScanningMode = true;
                backend.scan();
            }
        }
    }

    // --- 2. DEVICE LIST VIEW ---
    ColumnLayout {
        visible: mainWindow.isScanningMode
        anchors.fill: parent
        anchors.margins: 20

        // Header Row
        RowLayout {
            Layout.fillWidth: true
            Button {
                id: backButton
                text: "← Back"
                onClicked: mainWindow.isScanningMode = false
                background: Rectangle {
                    color: "transparent"
                }
                contentItem: Text {
                    text: backButton.text
                    color: "#58a6ff"
                    font.bold: true
                }
            }
            Text {
                text: "Discovered Devices (" + backend.foundDevices.length + ")"
                color: "white"
                font.pixelSize: 20
                font.bold: true
            }
            Item {
                Layout.fillWidth: true
            } // Spacer
        }

        // List View
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 15

            // Real Data Model from Backend
            model: backend.foundDevices

            // Using the delegate
            delegate: DeviceDelegate {}
        }
    }
}
