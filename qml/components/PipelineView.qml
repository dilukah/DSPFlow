import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: pipelineView
    width: 400
    height: 400
    color: "#2f2f2f"

    property var steps: []

    // Repeater dynamically creates StepView instances
    RowLayout {
        anchors.centerIn: parent
        spacing: 30

        Repeater {
            model: steps
            StepView {
                stepName: modelData.name
            }
        }
    }

    // Add new step function
    function addStep(stepName) {
        pipeline.addStep(stepName)
        steps = steps.concat({ name: stepName })
        console.log("Added step:", stepName)
    }
}
