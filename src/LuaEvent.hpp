#pragma once

#include <QtCore/QEvent>
#include <QtCore/QString>

class LuaEvent: public QEvent {
public:
    constexpr static QEvent::Type used_type = static_cast<QEvent::Type>(QEvent::User + 1);

    LuaEvent(QString name):
        QEvent(used_type), _name(name) {}

    QString name()const {
        return _name;
    }

private:
    QString _name;

};