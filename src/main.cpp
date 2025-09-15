#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "sakura-player.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 设置应用样式
    QQuickStyle::setStyle("Material");

    // 注册MpvPlayer类型到QML
    qmlRegisterType<SakuraPlayer>("org.bkcloud.SakuraPlayer", 1, 0, "SakuraPlayer");

    QQmlApplicationEngine engine {};
    engine.loadFromModule("QmlMpvPlayer", "Main");
    // engine.load("./main.qml");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
