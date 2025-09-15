import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    title: "Sakura Player"
    width: 800
    height: 600
    visible: true
    color: "black"

    property bool isPlaying: false
    property double duration: 0
    property double position: 0

    SakuraPlayer {
        id: sakuraPlayer
        anchors.fill: parent

        onDurationChanged: root.duration = duration
        onPositionChanged: root.position = position
        onPlayStateChanged: root.isPlaying = playing
    }

    // 控制栏
    Rectangle {
        id: controlBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 60
        color: Qt.rgba(0.1, 0.1, 0.1, 0.7)

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10

            // 播放/暂停按钮
            Button {
                icon.source: root.isPlaying ? "qrc:/icons/pause.png" : "qrc:/icons/play.png"
                onClicked: sakuraPlayer.togglePlayPause()
                background: Rectangle { color: "transparent" }
            }

            // 时间显示
            Label {
                text: formatTime(root.position) + " / " + formatTime(root.duration)
                color: "white"
                font.pixelSize: 12
            }

            // 进度条
            Slider {
                id: seekSlider
                Layout.fillWidth: true
                from: 0
                to: root.duration
                value: root.position
                onMoved: sakuraPlayer.seek(value)

                background: Rectangle {
                    implicitHeight: 4
                    color: "#424242"
                    radius: 2

                    Rectangle {
                        width: seekSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#1db954"
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: seekSlider.visualPosition * (parent.width - width)
                    y: (parent.height - height) / 2
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 8
                    color: seekSlider.pressed ? "#ffffff" : "#cccccc"
                }
            }

            // 音量控制
            Slider {
                id: volumeSlider
                from: 0
                to: 100
                value: 50
                onValueChanged: sakuraPlayer.setVolume(value)
                implicitWidth: 100

                background: Rectangle {
                    implicitHeight: 4
                    color: "#424242"
                    radius: 2

                    Rectangle {
                        width: volumeSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#1db954"
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: volumeSlider.visualPosition * (parent.width - width)
                    y: (parent.height - height) / 2
                    implicitWidth: 12
                    implicitHeight: 12
                    radius: 6
                    color: volumeSlider.pressed ? "#ffffff" : "#cccccc"
                }
            }

            // 打开文件按钮
            Button {
                icon.source: "qrc:/icons/folder.png"
                onClicked: fileDialog.open()
                background: Rectangle { color: "transparent" }
            }
        }
    }

    // 文件对话框
    FileDialog {
        id: fileDialog
        title: "Open Video File"
        nameFilters: ["Video Files (*.mp4 *.avi *.mkv *.mov *.webm)", "Audio Files (*.mp3 *.flac *.wav)", "All Files (*)"]
        onAccepted: sakuraPlayer.loadFile(selectedFile)
    }

    // 鼠标区域 - 用于显示/隐藏控制栏
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button === Qt.LeftButton) {
                controlBar.visible = !controlBar.visible
            } else if (mouse.button === Qt.RightButton) {
                contextMenu.popup()
            }
        }
        onDoubleClicked: {
            if (mouse.button === Qt.LeftButton) {
                root.visibility === Window.FullScreen ?
                    root.showNormal() : root.showFullScreen()
            }
        }
    }

    // 右键菜单
    Menu {
        id: contextMenu
        MenuItem {
            text: "Open File"
            onTriggered: fileDialog.open()
        }
        MenuItem {
            text: root.isPlaying ? "Pause" : "Play"
            onTriggered: sakuraPlayer.togglePlayPause()
        }
        MenuItem {
            text: root.visibility === Window.FullScreen ? "Exit Fullscreen" : "Fullscreen"
            onTriggered: {
                root.visibility === Window.FullScreen ?
                    root.showNormal() : root.showFullScreen()
            }
        }
        MenuItem {
            text: "Exit"
            onTriggered: Qt.quit()
        }
    }

    // 格式化时间函数
    function formatTime(seconds) {
        if (!seconds || seconds < 0) return "00:00:00"
        var hours = Math.floor(seconds / 3600)
        var minutes = Math.floor((seconds % 3600) / 60)
        var secs = Math.floor(seconds % 60)
        return Qt.formatTime(new Date(0, 0, 0, hours, minutes, secs), "hh:mm:ss")
    }
}
