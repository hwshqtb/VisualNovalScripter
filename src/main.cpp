#include <QtWidgets/QApplication>
#include "LuaMainWindow.hpp"
#include <QtCore/QFile>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    LuaMainWindow m;
    QFile file("../game.lua");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << file.errorString();
        return -1;
    }
    qDebug().nospace().noquote() << m.run(file.readAll()) << "\n====\n";
    m.show();

    return a.exec();
}
