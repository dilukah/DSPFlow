import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 120
    height: 60
    radius: 5
    color: active ? "#00aa00" : "#555555"
    border.color: "#222222"
    border.width: 2

    property alias stepName: nameText.text
    property bool active: false
    property string processingMode: "single"  // default mode
    property var extraParams: ({})            // for custom step parameters

    signal propertiesChanged(string mode, var params)

    Text {
        id: nameText
        anchors.centerIn: parent
        color: "white"
        font.bold: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: console.log("Clicked step:", stepName)
        onDoubleClicked: propertyPanel.open()
    }

    // ---------------- Property Panel ----------------
    Dialog {
        id: propertyPanel
        title: stepName + " Properties"
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        focus: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            ComboBox {
                id: modeSelector
                Layout.fillWidth: true
                model: ["Single", "Batch"]
                currentIndex: processingMode === "single" ? 0 : 1

                onCurrentIndexChanged: {
                       processingMode = currentText.toLowerCase() // update StepView property
                       propertiesChanged(processingMode, extraParams) // signal to Pipeline
                   }
            }

            TextField {
                id: paramField
                Layout.fillWidth: true
                placeholderText: "Parameter (optional)"
                text: extraParams.threshold !== undefined ? extraParams.threshold : ""
            }
        }

        onAccepted: {
            processingMode = modeSelector.currentText.toLowerCase()
            extraParams.threshold = paramField.text
            propertiesChanged(processingMode, extraParams)
            console.log("Updated step properties:", processingMode, extraParams)
        }
    }
}
