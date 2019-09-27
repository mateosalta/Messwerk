#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QObject>
#include <QString>
#include <QQuickView>
#include <QQmlContext>
#include <QGuiApplication>
#include <QTimer>
#include <QGeoSatelliteInfoSource>
#include <QQuickStyle>
#include <QQmlApplicationEngine> 
#include <QCoreApplication>
#include <QGuiApplication>



#include "accelerometer.h"
#include "gyroscope.h"
#include "magnetometer.h"
#include "rotation.h"
#include "light.h"
#include "pressuresensor.h"
#include "proximity.h"
#include "satelliteinfo.h"
#include "position.h"

#include "settings.h"

#include "plotwidget.h"
#include "satelliteposwidget.h"
#include "satellitestrengthwidget.h"


int main(int argc, char *argv[])
{
    int result = 0;

    QGuiApplication *app = new QGuiApplication(argc, (char**)argv);
    app->setApplicationName("harbour-messwerk.mymike00");
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
qputenv("QT_SCALE_FACTOR", "2.0");


    QQmlApplicationEngine engine;
            QQuickStyle::setStyle("Suru");

    qmlRegisterType<PlotWidget>("harbour.messwerk.MesswerkWidgets", 1, 0, "PlotWidget");
    qmlRegisterType<SatellitePosWidget>("harbour.messwerk.MesswerkWidgets", 1, 0, "SatellitePosWidget");
    qmlRegisterType<SatelliteStrengthWidget>("harbour.messwerk.MesswerkWidgets", 1, 0, "SatelliteStrengthWidget");

    QTimer refreshTimer;

    Accelerometer accelerometer(false);
    Gyroscope gyroscope(false);
    Magnetometer magnetometer(false);
    Rotation rotation(false);
    Light light(false);
    PressureSensor pressuresensor(false);
    Proximity proximity(true);
    SatelliteInfo satelliteinfo;
    Position position;

    // connect not self-refreshing sensors to the global timer
    QObject::connect(&refreshTimer, SIGNAL(timeout()), &accelerometer, SLOT(refresh()));
    QObject::connect(&refreshTimer, SIGNAL(timeout()), &pressuresensor, SLOT(refresh()));
    QObject::connect(&refreshTimer, SIGNAL(timeout()), &gyroscope, SLOT(refresh()));
    QObject::connect(&refreshTimer, SIGNAL(timeout()), &magnetometer, SLOT(refresh()));
    QObject::connect(&refreshTimer, SIGNAL(timeout()), &rotation, SLOT(refresh()));
    QObject::connect(&refreshTimer, SIGNAL(timeout()), &light, SLOT(refresh()));

    engine.rootContext()->setContextProperty("accelerometer", &accelerometer);
    engine.rootContext()->setContextProperty("gyroscope", &gyroscope);
    engine.rootContext()->setContextProperty("magnetometer", &magnetometer);
    engine.rootContext()->setContextProperty("rotationsensor", &rotation);
    engine.rootContext()->setContextProperty("lightsensor", &light);
    engine.rootContext()->setContextProperty("pressuresensor", &pressuresensor);
    engine.rootContext()->setContextProperty("proximitysensor", &proximity);
    engine.rootContext()->setContextProperty("satelliteinfo", &satelliteinfo);
    engine.rootContext()->setContextProperty("positionsensor", &position);
    engine.rootContext()->setContextProperty("settings", &(Settings::instance()));
    engine.load(QUrl(QStringLiteral("qrc:///qml/Messwerk.qml")));

    refreshTimer.start(100);

    result = app->exec();

    return result;
}
