#include <QtCore/QEvent>
#include <QtCore/QString>

class LuaEvent: public QEvent {
public:
    constexpr static QEvent::Type used_type = static_cast<QEvent::Type>(QEvent::User + 1);

    LuaEvent(std::string name):
        QEvent(used_type), _name(std::move(name)) {}

    std::string name()const {
        return _name;
    }

private:
    std::string _name;

};