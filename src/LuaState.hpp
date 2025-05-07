#include "luahelper.h"
#include <QtStateMachine/QState>

class LuaState: public QState {
    Q_OBJECT

public:
    LuaState(LuaFunc onEntry, LuaFunc onExit):
        QState(), _onEntry(onEntry), _onExit(onExit) {}

    LuaState(LuaFunc onEntry, LuaFunc onExit, QState* parent):
        QState(parent), _onEntry(onEntry), _onExit(onExit) {}

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