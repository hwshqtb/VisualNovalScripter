#include "luahelper.h"
#include <QtStateMachine/QStateMachine>

class LuaStateMachine: public QStateMachine {
    Q_OBJECT

public:
    LuaStateMachine(LuaFunc onEntry, LuaFunc onExit):
        QStateMachine(), _onEntry(onEntry), _onExit(onExit) {}

    LuaStateMachine(LuaFunc onEntry, LuaFunc onExit, QState* parent):
        QStateMachine(parent), _onEntry(onEntry), _onExit(onExit) {}

    void onEntry(QEvent*) override {
        _onEntry();
    }

    void onExit(QEvent*) override {
        _onExit();
    }

private:
    LuaFunc _onEntry;
    LuaFunc _onExit;
};