#pragma once
#include "Step.h"
#include <QObject>
#include <QVector>
#include <steps.h>
#include "stepFactory.h"
#include<QDebug>


class Pipeline : public QObject
{
    Q_OBJECT
public:
    explicit Pipeline(QObject* parent = nullptr)
        : QObject(parent), factory(new StepFactory(this)) {}

    Q_INVOKABLE void addStep(const QString& stepName) {
        Step* step = factory->createStep(stepName, this);
        if (step) {
            steps.append(step);
            qDebug() << "Added step:" << stepName;
        } else {
            qWarning() << "Unknown step:" << stepName;
        }
    }

    Q_INVOKABLE QStringList getAvailableSteps() const {
        return factory->availableSteps();
    }

    Q_INVOKABLE QVector<double> runPipeline() {
        QVector<double> data;

        for (Step* step : steps) {

            // If first step is a generator, decide based on its processingMode
            if (data.isEmpty() && step->inherits("GenerateDataStep")) {
                auto genStep = dynamic_cast<GenerateDataStep*>(step);
                if (genStep->processingMode() == "single") {
                    double value = genStep->execute(0.0);
                    data.append(value);
                } else {
                    data = genStep->executeBatch({});
                }
            }
            else if (data.size() == 1) {
                // single value from previous step, use execute()
                double value = step->execute(data[0]);
                data[0] = value;
            }
            else {
                // otherwise batch
                data = step->executeBatch(data);
            }

            // Emit signals
            if (auto plot = dynamic_cast<PlotStep*>(step))
                emit plotDataUpdated(plot->getData());

            if (auto branch = dynamic_cast<BranchStep*>(step))
                emit branchTaken(branch->getBranchTaken() == 1);
        }

        return data;
    }

    Q_INVOKABLE double runPipeline(double value) {
        double v = value;
        for (Step* step : steps) {
            v = step->execute(v);

            if (auto plot = dynamic_cast<PlotStep*>(step))
                emit plotDataUpdated(plot->getData());

            if (auto branch = dynamic_cast<BranchStep*>(step))
                emit branchTaken(branch->getBranchTaken() == 1);
        }
        return v;
    }

    Q_INVOKABLE void setStepMode(const QString& stepName, const QString& mode) {
        for (Step* s : steps) {
            if (s->getName() == stepName) {
                if (auto gen = dynamic_cast<GenerateDataStep*>(s))
                    gen->setProcessingMode(mode);
                break;
            }
        }
    }


signals:
    void plotDataUpdated(const QVector<double>& data);
    void branchTaken(bool branchHigh);

private:
    QVector<Step*> steps;
    StepFactory* factory;
};
