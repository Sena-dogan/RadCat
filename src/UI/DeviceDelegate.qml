import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: ListView.view.width
    height: 85 + (isExpanded ? 100 : 0)

    Behavior on height {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    required property string name
    required property string status

    property bool isExpanded: false

    Rectangle {
        id: cardBackground
        anchors.fill: parent
        anchors.margins: 5
        color: "#161b22" // Dark card background
        radius: 8
        border.color: "#30363d"
        border.width: 1

        ColumnLayout {
            id: contentCol
            width: parent.width
            spacing: 0

            // --- TOP SECTION (Always Visible) ---
            Rectangle {
                Layout.fillWidth: true
                height: 60
                color: "transparent"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15
                    spacing: 15

                    // Status Indicator Circle
                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        color: {
                            // Using the required property 'status' directly
                            if (root.status === "match")
                                return "#238636"; // Green
                            if (root.status === "partial")
                                return "#d29922"; // Yellow
                            return "#f85149"; // Red
                        }
                    }

                    // Device Name
                    Text {
                        text: root.name // Using required property 'name'
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                        Layout.fillWidth: true
                    }

                    // Activate Button
                    Button {
                        id: activateButton
                        visible: root.status !== "mismatch"
                        text: "Activate"

                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 36

                        background: Rectangle {
                            color: activateButton.down ? "#1d4ed8" : (activateButton.hovered ? "#3b82f6" : "#2563eb")
                            radius: 6
                            Behavior on color {
                                ColorAnimation {
                                    duration: 150
                                }
                            }
                        }
                        contentItem: Text {
                            text: activateButton.text
                            color: "white"
                            font.bold: true
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: console.log("Activated: " + root.name)

                        hoverEnabled: true
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                    }

                    // Expand/Collapse Arrow
                    Text {
                        text: root.isExpanded ? "▲" : "▼"
                        color: "#8b949e"
                        font.pixelSize: 12

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.isExpanded = !root.isExpanded
                        }
                    }
                }
            }

            // --- DETAILS SECTION (Collapsible) ---
            Rectangle {
                Layout.fillWidth: true
                // Toggle height based on expansion state
                height: root.isExpanded ? 100 : 0
                color: "#0d1117"
                clip: true // Clips content during animation

                // Smooth expansion animation
                Behavior on height {
                    NumberAnimation {
                        duration: 200
                    }
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 5
                    opacity: root.isExpanded ? 1.0 : 0.0

                    // Fade in/out animation
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                        }
                    }

                    // Placeholder details
                    Text {
                        text: "Vendor ID: 1234 | Product ID: 5678"
                        color: "#8b949e"
                    }
                    Text {
                        text: "Status: " + root.status
                        color: "#8b949e"
                    }
                }
            }
        }
    }
}
