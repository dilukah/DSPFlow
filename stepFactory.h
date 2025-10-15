#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <functional>
#include "Step.h"
#include "steps.h"

class StepFactory : public QObject
{
    Q_OBJECT
public:
    explicit StepFactory(QObject* parent = nullptr) : QObject(parent) {
        // Register all available steps
        registerStep("GenerateData", [] (QObject* parent) { return new GenerateDataStep(parent); });
        registerStep("Transform", [] (QObject* parent) { return new TransformStep(parent); });
        registerStep("Branch", [] (QObject* parent) { return new BranchStep(8.0, parent); });
        registerStep("Plot", [] (QObject* parent) { return new PlotStep(parent); });
        registerStep("FFT", [] (QObject* parent) { return new FFTStep(parent); });
        registerStep("LowPassFilter", [] (QObject* parent) { return new LowPassFilterStep(parent); });
    }

    Step* createStep(const QString& name, QObject* parent = nullptr) const {
        auto it = creators.find(name);
        if (it != creators.end())
            return it.value()(parent);
        return nullptr;
    }

    QStringList availableSteps() const {
        return creators.keys();
    }

private:
    void registerStep(const QString& name, std::function<Step*(QObject*)> constructor) {
        creators[name] = constructor;
    }

    QMap<QString, std::function<Step*(QObject*)>> creators;
};
