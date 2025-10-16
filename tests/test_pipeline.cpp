#include <QtTest>
#include "Pipeline.h"
#include "steps.h"

class PipelineTest : public QObject {
    Q_OBJECT

private slots:
    void testSingleValueGeneration() {
        Pipeline pipeline;
        pipeline.addStep("GenerateData");

        auto result = pipeline.runPipeline();
        QVERIFY(!result.isEmpty());
        QVERIFY(result[0] >= 0.0 && result[0] <= 10.0);
    }

    void testLowPassFilter() {
        Pipeline pipeline;
        pipeline.addStep("GenerateData");
        pipeline.addStep("LowPassFilter");

        auto result = pipeline.runPipeline();
        QVERIFY(!result.isEmpty());
    }
};

QTEST_MAIN(PipelineTest)
#include "test_pipeline.moc"
