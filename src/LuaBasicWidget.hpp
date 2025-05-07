#include "luahelper.h"
#include <QtWidgets/QWidget>
#include <QtGui/QFont>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtMultimedia/QSoundEffect>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimediaWidgets/QVideoWidget>

#include <QTimer>

class LuaBasicWidget: public QWidget {
    Q_OBJECT

public:
    LuaBasicWidget():
        QWidget(),
        _last(nullptr) {}

    LuaBasicWidget(QWidget* parent):
        QWidget(parent),
        _last(nullptr) {}

    void plainText(int x, int y, int width, int height,
        std::string text, int msec, std::string fontName, int fontSize, int fontWeight, bool fontItalic, std::string color, int lineDistance,
        std::string background, std::string audioPath, bool wait) {
        QString realText = QString::fromStdString(text);
        if (realText.isEmpty())
            return;
        auto widget = new QPlainTextEdit(this);
        widget->move(x, y);
        widget->resize(width, height);
        styleSheet(widget, background);

        QFont font(QString::fromStdString(fontName), fontSize, fontWeight, fontItalic);
        QTextCharFormat charFormat;
        charFormat.setForeground(QColor(QString::fromStdString(color)));
        charFormat.setFont(font);
        QTextBlockFormat blockFormat;
        blockFormat.setLineHeight(lineDistance, QTextBlockFormat::LineDistanceHeight);
        QTextCursor cursor = widget->textCursor();
        cursor.setBlockFormat(blockFormat);
        cursor.setCharFormat(charFormat);
        widget->setTextCursor(cursor);

        if (msec == 0)
            widget->setPlainText(realText);
        else {
            auto timer = new QTimer(this);
            timer->setSingleShot(true);
            timer->setProperty("index", 1);
            if (wait)
                widget->setProperty("wait", widget->property("wait").toInt() + 1);
            connect(timer, &QTimer::timeout, [=]() {
                int index = timer->property("index").toInt();
                while (index < realText.length() && realText[index].isSpace())
                    widget->appendPlainText(realText[index++]);
                if (index >= realText.length()) {
                    timer->deleteLater();
                    if (wait)
                        widget->setProperty("wait", widget->property("wait").toInt() - 1);
                    return;
                }
                widget->appendPlainText(realText[index]);
                timer->setProperty("index", index + 1);
                timer->start(msec);
            });
            timer->start(0);
        }

        audio(widget, audioPath)->play();
        _last->stackUnder(widget);
        _last = widget;
    }

    void RichText(int x, int y, int width, int height,
        std::vector<std::string> text, int msec,
        std::string background, std::string audioPath, bool wait) {
        if (text.empty())
            return;
        auto widget = new QPlainTextEdit(this);
        widget->move(x, y);
        widget->resize(width, height);
        styleSheet(widget, background);

        if (msec == 0) {
            
            widget->setPlainText(realText);
        }
        else {
            auto timer = new QTimer(this);
            timer->setSingleShot(true);
            timer->setProperty("index", 1);
            if (wait)
                widget->setProperty("wait", widget->property("wait").toInt() + 1);
            connect(timer, &QTimer::timeout, [=]() {
                int index = timer->property("index").toInt();
                while (index < realText.length() && realText[index].isSpace())
                    widget->appendPlainText(realText[index++]);
                if (index >= realText.length()) {
                    timer->deleteLater();
                    if (wait)
                        widget->setProperty("wait", widget->property("wait").toInt() - 1);
                    return;
                }
                widget->appendPlainText(realText[index]);
                timer->setProperty("index", index + 1);
                timer->start(msec);
            });
            timer->start(0);
        }

        audio(widget, audioPath)->play();
        _last->stackUnder(widget);
        _last = widget;
    }

private:
    static void styleSheet(QWidget* widget, std::string background) {
        if (background == "transparent") { // transparent
            widget->setAttribute(Qt::WA_TranslucentBackground);
            widget->setWindowFlag(Qt::FramelessWindowHint);
        }
        else if (background.front() == '!') { // color
            widget->setStyleSheet(QString("background-color: url(%1)").arg(QString::fromStdString(background).removeFirst()));
        }
        else { // picture
            widget->setStyleSheet(QString("background: url(%1)").arg(QString::fromStdString(background)));
        }
    }

    static QSoundEffect* audio(QWidget* parent, std::string path, int loop = 1) {
        auto audio = new QSoundEffect(parent);
        audio->setSource(QUrl::fromLocalFile(QString::fromStdString(path)));
        audio->setLoopCount(loop);
        return audio;
    }

private:
    QWidget* _last;

};