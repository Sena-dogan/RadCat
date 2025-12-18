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

    required property string deviceName
    required property int matchScore
    required property bool serialMatch
    required property bool nameMatch
    required property bool vidMatch
    required property bool pidMatch

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
                            if (root.matchScore >= 3)
                                return "#238636"; // Green (High match)
                            if (root.matchScore >= 1)
                                return "#d29922"; // Yellow (Partial)
                            return "#f85149"; // Red (Mismatch)
                        }
                    }

                    // Device Name
                    Text {
                        text: root.deviceName
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                        Layout.fillWidth: true
                    }

                    // Activate Button
                    Button {
                        id: activateButton
                        visible: root.matchScore > 1
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
                        onClicked: console.log("Activated: " + root.deviceName)

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

                    // Details
                    Text {
                        text: "Match Score: " + root.matchScore + "/3"
                        color: "#8b949e"
                    }
                    Text {
                        text: "Matches: " + (root.vidMatch ? "VID " : "") + (root.pidMatch ? "PID " : "") + (root.nameMatch ? "NAME " : "") + (root.serialMatch ? "SERIAL" : "")
                        color: "#8b949e"
                    }
                }
            }
        }
    }
}
