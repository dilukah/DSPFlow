import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts
import "components"
ApplicationWindow {
    visible: true
    width: 900
    height: 600
    title: qsTr("DSPFlow")

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        Button {
            text: "Add Step"
            Menu {
                id: stepMenu
                Repeater {
                    model: pipeline.getAvailableSteps()
                    MenuItem {
                        text: modelData
                        onTriggered: pipelineView.addStep(modelData)
                    }
                }
            }
            onClicked: stepMenu.open()
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter

            PipelineView {
                            id: pipelineView
                            Layout.alignment: Qt.AlignVCenter
                        }

            ChartView {
                id: chartView
                width: 500; height: 400
                antialiasing: true

                ValueAxis { id: axisX; min: 0; max: 10 }
                ValueAxis { id: axisY; min: 0; max: 10 }

                LineSeries {
                    id: lineSeries
                    axisX: axisX
                    axisY: axisY
                }
            }
        }

        RowLayout {
            spacing: 10

            Button {
                text: "Run Single Sample"
                onClicked: pipeline.runPipeline()
            }

            Button {
                text: "Run Batch"
                onClicked: {
                    var batch = []
                    for (var i=0; i<20; i++)
                        batch.push(Math.random() * 10)
                    pipeline.runPipeline()
                }
            }

            Button {
                text: "Reset Chart"
                onClicked: {
                    lineSeries.clear()
                    axisX.min = 0
                    axisX.max = 10
                    axisY.min = 0
                    axisY.max = 10
                }
            }

            Button {
                text: "Start Streaming"
                checkable: true
                onCheckedChanged: timer.running = checked
            }
        }

        TextArea {
            id: logArea
            width: parent.width
            height: 150
            readOnly: true
        }
    }

    Timer {
        id: timer
        interval: 500
        repeat: true
        running: false
        onTriggered: pipeline.runPipeline(Math.random() * 10)
    }

    Connections {
        target: pipeline

        function onPlotDataUpdated(newData) {
            lineSeries.clear()
            for (let i = 0; i < newData.length; ++i)
                lineSeries.append(i, newData[i])

            if (newData.length > 0) {
                axisX.min = 0
                axisX.max = newData.length - 1
                var minY = Math.min.apply(null, newData)
                var maxY = Math.max.apply(null, newData)
                axisY.min = minY - 1
                axisY.max = maxY + 1
            }

            logArea.append("Pipeline executed. Plot updated.")
        }

        function onBranchTaken(branchValue) {
            logArea.append("Branch taken: " + (branchValue ? "High" : "Low"))
        }
    }
}
