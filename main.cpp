#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "pipeline.h"
#include "steps.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //Create pipeline and add steps
    Pipeline* pipeline = new Pipeline();

    engine.rootContext()->setContextProperty("pipeline", pipeline);
    engine.loadFromModule("DSPFlow", "Main");

    return app.exec();
}
