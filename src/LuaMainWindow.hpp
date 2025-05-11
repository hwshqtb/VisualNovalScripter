#include "luahelper.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include "LuaBasicWidget.hpp"
#include <QtStateMachine/QAbstractState>
#include "LuaState.hpp"
#include "LuaStateMachine.hpp"
#include "LuaEvent.hpp"
#include "LuaTransition.hpp"

class LuaMainWindow: public QMainWindow {
public:
    LuaMainWindow():
        L(luaL_newstate()), machine(nullptr) {
        luaL_requiref(L, "_G", luaopen_base, 1);
        lua_pop(L, 1);
        luaL_requiref(L, LUA_LOADLIBNAME, luaopen_package, 1);
        lua_pop(L, 1);
        luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
        lua_pop(L, 1);
        luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
        lua_pop(L, 1);

        luabridge::getGlobalNamespace(L)
            .beginNamespace("FSM")
                .beginClass<QAbstractState>("QAbstractState")
                .endClass()
                .deriveClass<QState, QAbstractState>("QState")
                .endClass()
                .deriveClass<QStateMachine, QState>("QState")
                .endClass()
                .deriveClass<LuaState, QState>("state")
                .addConstructor<void(LuaFunc, LuaFunc, QState*)>()
                .endClass()
                .deriveClass<LuaStateMachine, QStateMachine>("machine")
                .addConstructor<void(LuaFunc, LuaFunc), void(LuaFunc, LuaFunc, QState*)>()
                .addFunction("init_state", &LuaStateMachine::setInitialState)
                .endClass()
                .beginClass<LuaTransition>("transition")
                .addConstructor<void(LuaFunc, LuaFunc, QState*, QState*)>()
                .endClass()
                .endNamespace()
                .beginClass<QWidget>("QWidget")
                .endClass()
            .beginNamespace("window")
            .addFunction("resize", [this](int width, int height) {
            this->setFixedSize(width, height);
        })
            .addFunction("title", [this](std::string name, std::string iconPath) {
            this->setWindowTitle(QString::fromStdString(name));
            this->setWindowIcon(QIcon(QString::fromStdString(iconPath)));
        })
            .addFunction("move_cursor", [this](int x, int y) {
                this->cursor().setPos(x, y);
            })
            .addFunction("cursor_image", [this](std::string path) {
            QCursor cursor(QPixmap("../" + QString::fromStdString(path)).scaled(32, 32), 0, 0);
            this->setCursor(cursor);
        })
            .addFunction("audio", [this](std::string path, double audioVolume, int loop) {
                LuaBasicWidgetHelper::audio(this, path, audioVolume, loop);
            })
            .addFunction("set_FSM", [this](LuaStateMachine* m) {
            machine = m;
        })
            .endNamespace()
            .deriveClass<LuaBasicWidget, QWidget>("frame")
            .addConstructor([this](void* ptr, std::string name) {
            auto widget = new(ptr) LuaBasicWidget(this);
            this->setCentralWidget(widget);
            qDebug().nospace().noquote() << "Frame: " << name << "\n";
            luabridge::setGlobal(L, widget, name.c_str());
            connect(widget, &LuaBasicWidget::luaEventAction, this, [this](QString action) {
                if (machine)
                    machine->postEvent(new LuaEvent(action));
            });
            qDebug() << luabridge::getGlobal(L, name.c_str()).unsafe_cast<LuaBasicWidget*>();
            return widget;
        })
            .addFunction("plain_text", &LuaBasicWidget::plainText)
            .addFunction("rich_text", &LuaBasicWidget::richText)
            .addFunction("image", &LuaBasicWidget::image)
            .addFunction("video", &LuaBasicWidget::video)
            .addFunction("button", &LuaBasicWidget::button)
            .addFunction("audio", [this](LuaBasicWidget* widget, std::string path, double audioVolume, int loop) {
            LuaBasicWidgetHelper::audio(widget, path, audioVolume, loop);
        })
            .endClass();

        luabridge::getGlobal(L, "package")["path"] = "../?.lua;" + luabridge::getGlobal(L, "package")["path"].unsafe_cast<std::string>();
    }

    ~LuaMainWindow() {
        lua_close(L);
    }

    QString run(QByteArray script) {
        luaL_dostring(L, script.constData());
        if (lua_isstring(L, -1)) {
            QString result(lua_tostring(L, -1));
            lua_pop(L, 1);
            return result;
        }
        if (machine)
            machine->start();
        return "";
    }

private:
    lua_State* L;
    LuaStateMachine* machine;

};