import QtQuick
import QtQuick.Controls

MenuBar {
    Menu {
        title: "&File"
        
        Action { 
            text: "&New Session"
            shortcut: "StandardKey.New"
            onTriggered: console.log("New session triggered")
        }
        Action { 
            text: "&Open Log..."
            shortcut: "StandardKey.Open"
            onTriggered: console.log("Open log triggered")
        }
        MenuSeparator { }
        Action {
            text: "&Quit"
            shortcut: "StandardKey.Quit"
            onTriggered: Qt.quit()
        }
    }

    Menu {
        title: "&Edit"
        Action { 
            text: "Cut"
            shortcut: "StandardKey.Cut"
            onTriggered: console.log("Cut triggered")
        }
        Action { 
            text: "Copy"
            shortcut: "StandardKey.Copy"
            onTriggered: console.log("Copy triggered")
        }
        Action { 
            text: "Paste"
            shortcut: "StandardKey.Paste"
            onTriggered: console.log("Paste triggered")
        }
    }

    Menu {
        title: "&View"
        MenuItem {
            text: "Show Grid"
            checkable: true
            checked: true
            onTriggered: console.log("Grid toggled: " + checked)
        }
        MenuSeparator { }
        Action {
            text: "Zoom In"
            shortcut: "StandardKey.ZoomIn"
            onTriggered: console.log("Zoom in triggered")
        }
    }

    Menu {
        title: "&Help"
        Action {
            text: "About LabControl"
            onTriggered: console.log("About triggered")
        }
    }
}