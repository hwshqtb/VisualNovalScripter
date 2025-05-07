#include <QApplication>
#include "LuaMainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    LuaMainWindow m;
    m.show();

    return a.exec();
}
