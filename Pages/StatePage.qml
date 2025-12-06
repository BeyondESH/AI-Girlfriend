import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../Components"

Page {
    id: statePage
    
    property bool ollamaOnline: false
    property bool asrOnline: false
    property bool ttsOnline: false
    property string ollamaInfo: "检测中..."
    property string asrInfo: "检测中..."
    property string ttsInfo: "检测中..."

    background: Rectangle {
        color: "#f5f5f5"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 24

        // 页面标题
        Label {
            text: "服务器状态"
            font.family: "Microsoft YaHei"
            font.pixelSize: 28
            font.bold: true
            color: "#333333"
        }

        Label {
            text: "实时监控各项服务的运行状态"
            font.family: "Microsoft YaHei"
            font.pixelSize: 14
            color: "#666666"
            Layout.bottomMargin: 10
        }

        // Ollama 服务状态卡片
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            radius: 16
            color: "#FFFFFF"
            border.color: ollamaOnline ? "#4CAF50" : "#E0E0E0"
            border.width: ollamaOnline ? 2 : 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                // 状态指示灯
                Rectangle {
                    width: 60
                    height: 60
                    radius: 30
                    color: ollamaOnline ? "#E8F5E9" : "#FFEBEE"

                    Rectangle {
                        width: 20
                        height: 20
                        radius: 10
                        anchors.centerIn: parent
                        color: ollamaOnline ? "#4CAF50" : "#F44336"

                        SequentialAnimation on opacity {
                            running: true
                            loops: Animation.Infinite
                            NumberAnimation { from: 1; to: 0.5; duration: 1000 }
                            NumberAnimation { from: 0.5; to: 1; duration: 1000 }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Label {
                        text: "Ollama LLM 服务"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    Label {
                        text: ollamaInfo
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 13
                        color: ollamaOnline ? "#4CAF50" : "#F44336"
                    }

                    Label {
                        text: "地址: " + configMgr.llmServerUrl
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 12
                        color: "#999999"
                    }
                }

                Button {
                    text: "刷新"
                    font.family: "Microsoft YaHei"
                    onClicked: {
                        ollamaInfo = "检测中..."
                        app.checkOllamaStatus()
                    }
                }
            }
        }

        // FunASR 服务状态卡片
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            radius: 16
            color: "#FFFFFF"
            border.color: asrOnline ? "#4CAF50" : "#E0E0E0"
            border.width: asrOnline ? 2 : 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Rectangle {
                    width: 60
                    height: 60
                    radius: 30
                    color: asrOnline ? "#E8F5E9" : "#FFEBEE"

                    Rectangle {
                        width: 20
                        height: 20
                        radius: 10
                        anchors.centerIn: parent
                        color: asrOnline ? "#4CAF50" : "#F44336"

                        SequentialAnimation on opacity {
                            running: true
                            loops: Animation.Infinite
                            NumberAnimation { from: 1; to: 0.5; duration: 1000 }
                            NumberAnimation { from: 0.5; to: 1; duration: 1000 }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Label {
                        text: "FunASR 语音识别服务"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    Label {
                        text: asrInfo
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 13
                        color: asrOnline ? "#4CAF50" : "#F44336"
                    }

                    Label {
                        text: "地址: " + configMgr.asrServerUrl
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 12
                        color: "#999999"
                    }
                }

                Button {
                    text: "刷新"
                    font.family: "Microsoft YaHei"
                    onClicked: {
                        asrInfo = "检测中..."
                        app.checkAsrStatus()
                    }
                }
            }
        }

        // CosyVoice 服务状态卡片
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            radius: 16
            color: "#FFFFFF"
            border.color: ttsOnline ? "#4CAF50" : "#E0E0E0"
            border.width: ttsOnline ? 2 : 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Rectangle {
                    width: 60
                    height: 60
                    radius: 30
                    color: ttsOnline ? "#E8F5E9" : "#FFEBEE"

                    Rectangle {
                        width: 20
                        height: 20
                        radius: 10
                        anchors.centerIn: parent
                        color: ttsOnline ? "#4CAF50" : "#F44336"

                        SequentialAnimation on opacity {
                            running: true
                            loops: Animation.Infinite
                            NumberAnimation { from: 1; to: 0.5; duration: 1000 }
                            NumberAnimation { from: 0.5; to: 1; duration: 1000 }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Label {
                        text: "CosyVoice 语音合成服务"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    Label {
                        text: ttsInfo
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 13
                        color: ttsOnline ? "#4CAF50" : "#F44336"
                    }

                    Label {
                        text: "地址: " + configMgr.ttsServerUrl
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 12
                        color: "#999999"
                    }
                }

                Button {
                    text: "刷新"
                    font.family: "Microsoft YaHei"
                    onClicked: {
                        ttsInfo = "检测中..."
                        app.checkTtsStatus()
                    }
                }
            }
        }

        // 一键刷新按钮
        Button {
            text: "刷新所有服务状态"
            font.family: "Microsoft YaHei"
            font.pixelSize: 14
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 20
            onClicked: {
                ollamaInfo = "检测中..."
                asrInfo = "检测中..."
                ttsInfo = "检测中..."
                app.checkOllamaStatus()
                app.checkAsrStatus()
                app.checkTtsStatus()
            }
        }

        Item { Layout.fillHeight: true }
    }

    // 信号连接
    Connections {
        target: app
        function onOllamaStatusChanged(online, info) {
            ollamaOnline = online
            ollamaInfo = info
        }
        function onAsrStatusChanged(online, info) {
            asrOnline = online
            asrInfo = info
        }
        function onTtsStatusChanged(online, info) {
            ttsOnline = online
            ttsInfo = info
        }
    }

    // 页面加载时自动检测
    Component.onCompleted: {
        app.checkOllamaStatus()
        app.checkAsrStatus()
        app.checkTtsStatus()
    }

    // 定时刷新
    Timer {
        interval: 30000 // 30秒刷新一次
        running: true
        repeat: true
        onTriggered: {
            app.checkOllamaStatus()
            app.checkAsrStatus()
            app.checkTtsStatus()
        }
    }
}
