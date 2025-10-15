#pragma once
#include "step.h"
#include <cstdlib>
#include <cmath>
#include <complex>
#include <vector>

class GenerateDataStep : public Step
{
    Q_OBJECT
    Q_PROPERTY(QString processingMode READ processingMode WRITE setProcessingMode NOTIFY processingModeChanged)

public:
    GenerateDataStep(QObject *parent = nullptr)
        : Step("GenerateData", parent), m_processingMode("batch"), m_batchSize(10) {}

    // Single value processing
    double execute(double /*input*/) override {
        return generateSample();
    }

    // Batch processing
    QVector<double> executeBatch(const QVector<double>& input) override {
        Q_UNUSED(input);
        QVector<double> output;

        if (m_processingMode == "single") {
            output.append(generateSample());
        } else {
            output.reserve(m_batchSize);
            for (int i = 0; i < m_batchSize; ++i)
                output.append(generateSample());
        }
        return output;
    }

    QString processingMode() const { return m_processingMode; }
    void setProcessingMode(const QString& mode) {
        if (mode != m_processingMode) {
            m_processingMode = mode;
            emit processingModeChanged();
        }
    }

    void setBatchSize(int n) { m_batchSize = n; }

signals:
    void processingModeChanged();

private:
    QString m_processingMode; // "single" or "batch"
    int m_batchSize;

    double generateSample() {
        return (rand() % 100) / 10.0; // 0-10
    }
};

class TransformStep : public Step
{
    Q_OBJECT
public:
    TransformStep(QObject *parent = nullptr) : Step("Transform", parent) {}

    double execute(double input) override {
        return input * 2;
    }

    // Optional batch override (more efficient than per-element default)
    QVector<double> executeBatch(const QVector<double>& input) override {
        QVector<double> output;
        output.reserve(input.size());
        for (double v : input)
            output.append(v * 2);
        return output;
    }
};

class BranchStep : public Step
{
    Q_OBJECT
public:
    BranchStep(double threshold = 5.0, QObject *parent = nullptr)
        : Step("Branch", parent), threshold(threshold), branchTaken(0) {}

    double execute(double input) override {
        branchTaken = input > threshold ? 1 : 0;
        return input;
    }

    QVector<double> executeBatch(const QVector<double>& input) override {
        QVector<double> output;
        output.reserve(input.size());
        for (double v : input) {
            output.append(execute(v));
        }
        return output;
    }

    int getBranchTaken() const { return branchTaken; }

private:
    double threshold;
    int branchTaken;
};

class PlotStep : public Step
{
    Q_OBJECT
public:
    PlotStep(QObject *parent = nullptr) : Step("Plot", parent) {}

    double execute(double input) override {
        outputData.append(input);
        return input;
    }

    QVector<double> executeBatch(const QVector<double>& input) override {
        outputData += input; // append entire batch
        return input;
    }

    QVector<double> getData() const { return outputData; }

private:
    QVector<double> outputData;
};


class FFTStep : public Step
{
    Q_OBJECT
public:
    FFTStep(QObject *parent = nullptr) : Step("FFT", parent) {}

    double execute(double input) override {
        buffer.push_back(input);
        if (buffer.size() >= windowSize) {
            performFFT();
            buffer.clear();
        }
        return input;
    }

    QVector<double> executeBatch(const QVector<double>& input) override {
        for (double v : input)
            execute(v);  // fill buffer and compute FFT in windows
        return input;
    }

    QVector<double> getMagnitudeSpectrum() const { return magnitude; }

private:
    std::vector<double> buffer;
    QVector<double> magnitude;
    const size_t windowSize = 32;

    void performFFT() {
        size_t N = buffer.size();
        std::vector<std::complex<double>> X(N);
        for (size_t k = 0; k < N; ++k) {
            std::complex<double> sum(0, 0);
            for (size_t n = 0; n < N; ++n) {
                double angle = -2.0 * M_PI * k * n / N;
                sum += std::polar(buffer[n], angle);
            }
            X[k] = sum;
        }

        magnitude.resize(N / 2);
        for (size_t i = 0; i < N / 2; ++i)
            magnitude[i] = std::abs(X[i]);
    }
};


class LowPassFilterStep : public Step
{
    Q_OBJECT
    Q_PROPERTY(double alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)

public:
    explicit LowPassFilterStep(QObject *parent = nullptr)
        : Step("LowPassFilter", parent), m_alpha(0.1), y_prev(0.0) {}

    double alpha() const { return m_alpha; }
    void setAlpha(double a) {
        if (qFuzzyCompare(a, m_alpha)) return;
        m_alpha = a;
        emit alphaChanged();
    }

    double execute(double input) override {
        double y = m_alpha * input + (1 - m_alpha) * y_prev;
        y_prev = y;
        return y;
    }

signals:
    void alphaChanged();

private:
    double m_alpha;
    double y_prev;
};
