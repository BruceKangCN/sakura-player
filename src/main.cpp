#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "sakura-player.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 设置应用样式
    QQuickStyle::setStyle("Material");

    // 注册MpvQtPlayer类型到QML
    constexpr const char* APP_URL = "org.bkcloud.SakuraPlayer";
    constexpr auto APP_VERSION_MAJOR = 1;
    constexpr auto APP_VERSION_MINOR = 0;
    constexpr const char* APP_NAME = "SakuraPlayer";
    qmlRegisterType<SakuraPlayer>(
        APP_URL,
        APP_VERSION_MAJOR,
        APP_VERSION_MINOR,
        APP_NAME
    );

    QQmlApplicationEngine engine {};
    const QUrl url(u"qrc:/main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
