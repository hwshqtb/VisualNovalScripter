#pragma once

#include "luahelper.h"
#include <QtWidgets/QWidget>
#include <QtGui/QFont>
#include <QtWidgets/QPlainTextEdit>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtMultimedia/QSoundEffect>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QTimer>
#include <QtGui/QMouseEvent>

struct LuaBasicWidgetHelper {
    static void audio(QWidget* parent, std::string path, double audioVolume, int loop) {
        if (path.empty() || audioVolume <= 0 || loop == 0)
            return;
        auto player = new QMediaPlayer(parent);
        auto audio = new QAudioOutput(parent);
        player->setAudioOutput(audio);
        player->setSource(QUrl::fromLocalFile("../" + QString::fromStdString(path)));
        player->setLoops(loop);
        audio->setVolume(audioVolume);
        player->play();
    }

    static void setBackground(QWidget* widget, std::string background) {
        if (background.empty())
            return;
        if (background == "transparent") { // transparent
            widget->setAttribute(Qt::WA_TranslucentBackground);
            widget->setWindowFlag(Qt::FramelessWindowHint);
        }
        else if (background.front() == '!') { // color
            widget->setStyleSheet(QString("background-color: url(%1)").arg(QString::fromStdString(background).removeFirst()));
        }
        else { // picture
            widget->setStyleSheet(QString("background: url(%1)").arg("../" + QString::fromStdString(background)));
        }
    }
};

class LuaBasicWidget: public QWidget {
    Q_OBJECT

public:
    LuaBasicWidget(QWidget* parent = nullptr): 
        QWidget(parent), _wait(0) {}

signals:
    void luaEventAction(QString action);

public:
    QWidget* plainText(int x, int y, int width, int height,
        std::string text, int msec, std::string fontName, int fontSize, int fontWeight, bool fontItalic, std::string color, int lineDistance,
        std::string background, std::string audioPath, double audioVolume, bool wait) {
        QString realText = QString::fromStdString(text);
        if (realText.isEmpty())
            return nullptr;
        auto widget = new QPlainTextEdit(this);
        widget->move(x, y);
        widget->resize(width, height);
        LuaBasicWidgetHelper::setBackground(widget, background);

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
            timer->setProperty("index", 0);
            if (wait)
                ++_wait;
            connect(timer, &QTimer::timeout, [=]() {
                int index = timer->property("index").toInt();
                while (index < realText.length() && realText[index].isSpace())
                    widget->appendPlainText(realText[index++]);
                if (index >= realText.length()) {
                    timer->deleteLater();
                    if (wait)
                        --_wait;
                    return;
                }
                widget->appendPlainText(realText[index]);
                timer->setProperty("index", index + 1);
                timer->start(msec);
            });
            timer->start(0);
        }

        LuaBasicWidgetHelper::audio(widget, audioPath, audioVolume, 1);
        return widget;
    }

    QWidget* richText(int x, int y, int width, int height,
        std::vector<std::string> text, int msec,
        std::string background, std::string audioPath, double audioVolume, bool wait) {
        if (text.empty())
            return nullptr;
        auto widget = new QPlainTextEdit(this);
        widget->move(x, y);
        widget->resize(width, height);
        LuaBasicWidgetHelper::setBackground(widget, background);

        if (msec == 0) {
            for (auto&& str : text) {
                if (str.empty())
                    widget->appendHtml("<p></p>");
                else
                    widget->appendHtml(QString::fromStdString(str));
            }
        }
        else {
            auto timer = new QTimer(this);
            timer->setSingleShot(true);
            timer->setProperty("index", 0);
            if (wait)
                ++_wait;
            connect(timer, &QTimer::timeout, [=]() {
                int index = timer->property("index").toInt();
                while (index < text.size() && text[index].empty()) {
                    widget->appendHtml("<p></p>");
                    ++index;
                }
                if (index >= text.size()) {
                    timer->deleteLater();
                    if (wait)
                        --_wait;
                    return;
                }
                widget->appendHtml(QString::fromStdString(text[index]));
                timer->setProperty("index", index + 1);
                timer->start(msec);
            });
            timer->start(0);
        }

        LuaBasicWidgetHelper::audio(widget, audioPath, audioVolume, 1);
        return widget;
    }

    QWidget* image(int x, int y, int width, int height,
        std::string path, float alpha) {
        auto widget = new QLabel(this);
        widget->move(x, y);
        widget->resize(width, height);
        widget->setPixmap(QPixmap::fromImage(QImage("../" + QString::fromStdString(path))).scaled(width, height));
        widget->setAttribute(Qt::WA_TranslucentBackground);
        widget->setWindowFlag(Qt::FramelessWindowHint);
        widget->setWindowOpacity(alpha);
        return widget;
    }

    QWidget* video(int x, int y, int width, int height,
        std::string path, int loop, bool wait) {
        auto widget = new QVideoWidget(this);
        widget->move(x, y);
        widget->resize(width, height);
        widget->setAttribute(Qt::WA_TranslucentBackground);
        widget->setWindowFlag(Qt::FramelessWindowHint);

        auto player = new QMediaPlayer(this);
        auto audio = new QAudioOutput(this);
        player->setVideoOutput(widget);
        player->setAudioOutput(audio);
        audio->setVolume(100);
        player->setSource(QUrl::fromLocalFile("../" + QString::fromStdString(path)));
        player->setLoops(loop);
        if (wait) {
            ++_wait;
            connect(player, &QMediaPlayer::playingChanged, [=](bool playing) {
                if (!playing) {
                    player->deleteLater();
                    if (wait)
                        --_wait;
                }
            });
        }
        player->play();
        return widget;
    }

    QWidget* button(int x, int y, int width, int height,
        std::string text,
        std::string background, std::string audioPath, double audioVolume,
        std::string action) {
        auto widget = new QPushButton(this);
        widget->move(x, y);
        widget->resize(width, height);
        widget->setText(QString::fromStdString(text));
        LuaBasicWidgetHelper::setBackground(widget, background);

        if (audioPath.empty() || audioVolume <= 0) {
            connect(widget, &QPushButton::clicked, [=]() {
                emit luaEventAction(QString::fromStdString(action));
            });
        }
        else {
            auto player = new QMediaPlayer(this);
            auto audio = new QAudioOutput(this);
            player->setAudioOutput(audio);
            player->setSource(QUrl::fromLocalFile("../" + QString::fromStdString(audioPath)));
            player->setLoops(1);
            audio->setVolume(audioVolume);

            connect(widget, &QPushButton::clicked, [=]() {
                player->play();
                emit luaEventAction(QString::fromStdString(action));
            });
        }
        return widget;
    }

protected slots:
    void mouseReleaseEvent(QMouseEvent* event) override {
        if (_wait == 0) {
            emit this->luaEventAction("skip");
            event->accept();
        }
        else
            QWidget::mouseReleaseEvent(event);
    }

private:
    int _wait;

};