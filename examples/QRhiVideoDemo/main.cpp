#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/com/QRhiVideoDemo/main.qml")));

    return app.exec();
}
