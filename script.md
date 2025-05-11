# 概述
## FSM（有限状态自动机）
描述游戏整体的工具，由state（节点）和transition（转移）构成。

state可以是state或FSM

FSM由信号驱动，即transition会接受信号，之后判断

## frame（帧）
用于显示的基本单位，可以显示文字，图片，视频，播放音乐以及放置按钮。

按钮点击后可以发出信号。

默认情况下，在点击窗口时会发出`skip`信号

## 坐标
所有坐标均相对于窗口左上角，水平向右为x轴正方向，垂直向下为y轴正方向。

# Lua脚本描述
## frame部分
下方描述中，如无注明，与之前出现过的相同
- `_G.frame(name)` 在全局表`_G`中创建一个名为name的frame
- `f:plain_text(x, y, width, height, text, msec, fontName, fontSize, fontWeight, fontItalic, color, lineDistance, background, audioPath, audioVolume, wait)` 在(x,y)处在width*height的矩形内显示内容为text的文字。
    - 整数msec表示每个可见文字的显示间隔，单位为毫秒，不可见文字会直接显示。
    - fontName, fontSize, fontWeight, fontItalic, color, lineDistance分别描述字体名，字号，粗细，是否为斜体，字色，行间距。
    - background为背景
    - audioPath为音乐路径，audioVolume为音量
    - wait为true则直到文字显示完，禁用点击窗口
- `f:rich_text(x, y, width, height, texts, msec, background, audioPath, audioVolume, wait)` 在(x,y)处在width*height的矩形内显示内容为texts的富文字。
- `f:image(x, y, width, height, path, alpha)`在(x,y)处在width*height的矩形内显示路径为path的图片，其透明度为0到1之间的实数alpha。
- `f:video(x, y, width, height, path, loop, wait)`在(x,y)处在width*height的矩形内显示路径为path的视频。
- `f:button(x, y, width, height, text, background, audioPath, audioVolume, action)`在(x,y)处在width*height的矩形内显示按钮，当点击后发出action信号。

## 表 _G.FSM
- `FSM.machine(on_entry, on_exit, parent)` 创建一个状态机，当进入这个状态机时执行函数on_entry，退出这个状态机时执行函数on_exit，父state为parent。
    - parent可以为nil，此时表示为顶级FSM。
- `FSM.state(on_entry, on_exit, parent)` 创建一个节点，当进入这个节点时执行函数on_entry，退出这个节点时执行函数on_exit，父state为parent。
- `FSM.transition(trigger, transition, source, target)` 创建一个转移，源头为source，目标为target，通过函数trigger判断，发生转移时执行函数transition。

## 表 _G.window 全局管理
- `window.resize(width, height)` 将窗口固定为width*height大小
- `window.title(name, path)` 设置窗口的名称为name，图标为路径为path的图片
- `window.cursor_image(path)` 设置鼠标图片为路径为path的图片
- `window.move_cursor(x, y)` 将鼠标位置移动到(x,y)的位置
- `window.audio(path, volume, loop)` 以音量volume播放loop次路径为path的音乐
    - volume是0到1间的数
    - loop为-1代表一直播放
- `window.set_FSM(machine)` 设置全局状态机为machine