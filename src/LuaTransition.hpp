#include "luahelper.h"
#include <QtStateMachine/QAbstractTransition>
#include "LuaEvent.hpp"

class LuaTransition: public QAbstractTransition {
    Q_OBJECT

public:
    LuaTransition(LuaFunc trigger, LuaFunc transition):
        QAbstractTransition(),
        _trigger(trigger), _transition(transition) {}

    LuaTransition(LuaFunc trigger, LuaFunc transition, QState* source):
        QAbstractTransition(source),
        _trigger(trigger), _transition(transition) {}

    bool eventTest(QEvent* event) {
        if (event->type() == LuaEvent::used_type) {
            return _trigger(static_cast<LuaEvent*>(event)->name())[0].unsafe_cast<bool>();
        }
    }

    void onTransition(QEvent* event) {
        _transition(static_cast<LuaEvent*>(event)->name());
    }
    
private:
    LuaFunc _trigger;
    LuaFunc _transition;

};