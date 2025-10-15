#ifndef STEP_H
#define STEP_H

#include <QObject>
#include <QString>
#include <QVector>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(const QString &name, QObject *parent = nullptr)
        : QObject(parent), name(name) {}
    virtual ~Step() = default;

    // Streaming: process one sample
    virtual double execute(double input) = 0;

    // Batch: process a vector (default: call execute() per element)
    virtual QVector<double> executeBatch(const QVector<double>& input) {
        QVector<double> output;
        for (double v : input) {
            output.append(execute(v));
        }
        return output;
    }

    QString getName() const { return name; }

protected:
    QString name;
};

#endif // STEP_H
