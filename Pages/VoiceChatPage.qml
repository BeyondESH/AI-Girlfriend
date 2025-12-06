import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../Components"
import "../js/addListModel.js" as AddListModel

Page {
    id: voiceChatPage

    // 录音状态
    property bool isRecording: false
    property bool isProcessing: false
    property string currentTranscript: ""

    signal signal_sendVoiceMessage(string text)

    background: Rectangle {
        color: "#f5f5f5"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 对话列表区域
        ListView {
            id: chatListView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20
            model: voiceChatMessageModel
            spacing: 16
            clip: true

            // 自动滚动到底部
            onCountChanged: {
                Qt.callLater(function() {
                    chatListView.positionViewAtEnd()
                })
            }

            delegate: Item {
                width: chatListView.width
                height: messageBubble.height + 10

                Column {
                    id: messageBubble
                    width: parent.width
                    spacing: 4

                    // 角色名称
                    Label {
                        id: roleLabel
                        text: model.role === "User" ? "你" : "雅萱"
                        font.family: "Microsoft YaHei"
                        font.pointSize: 9
                        color: "#666666"
                        visible: model.role !== ""
                        anchors.left: model.role === "User" ? undefined : parent.left
                        anchors.right: model.role === "User" ? parent.right : undefined
                        anchors.leftMargin: model.role === "User" ? 0 : 16
                        anchors.rightMargin: model.role === "User" ? 16 : 0
                    }

                    // 消息气泡
                    Rectangle {
                        id: bubbleRect
                        width: Math.min(messageText.implicitWidth + 32, chatListView.width * 0.7)
                        height: messageText.implicitHeight + 24
                        radius: 16
                        color: model.role === "User" ? "#007AFF" : "#FFFFFF"
                        border.color: model.role === "User" ? "transparent" : "#E0E0E0"
                        border.width: model.role === "User" ? 0 : 1

                        anchors.left: model.role === "User" ? undefined : parent.left
                        anchors.right: model.role === "User" ? parent.right : undefined
                        anchors.leftMargin: model.role === "User" ? 0 : 12
                        anchors.rightMargin: model.role === "User" ? 12 : 0

                        // 添加阴影效果
                        layer.enabled: true
                        layer.effect: null

                        Label {
                            id: messageText
                            anchors.centerIn: parent
                            width: Math.min(implicitWidth, chatListView.width * 0.7 - 32)
                            text: model.context
                            font.family: "Microsoft YaHei"
                            font.pointSize: 12
                            color: model.role === "User" ? "#FFFFFF" : "#333333"
                            wrapMode: Text.Wrap
                            lineHeight: 1.4
                        }
                    }

                    // 时间戳（可选）
                    Label {
                        text: model.timestamp || ""
                        font.family: "Microsoft YaHei"
                        font.pointSize: 8
                        color: "#999999"
                        visible: model.timestamp !== undefined && model.timestamp !== ""
                        anchors.left: model.role === "User" ? undefined : parent.left
                        anchors.right: model.role === "User" ? parent.right : undefined
                        anchors.leftMargin: model.role === "User" ? 0 : 16
                        anchors.rightMargin: model.role === "User" ? 16 : 0
                    }
                }
            }

            // 空状态提示
            Label {
                anchors.centerIn: parent
                text: "点击下方麦克风开始语音对话"
                font.family: "Microsoft YaHei"
                font.pointSize: 14
                color: "#999999"
                visible: chatListView.count === 0
            }
        }

        // 实时转写显示区域
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: transcriptLabel.visible ? transcriptLabel.implicitHeight + 30 : 0
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            color: "#E8F5E9"
            radius: 12
            visible: currentTranscript !== "" && isRecording

            Behavior on Layout.preferredHeight {
                NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
            }

            Label {
                id: transcriptLabel
                anchors.centerIn: parent
                width: parent.width - 40
                text: currentTranscript
                font.family: "Microsoft YaHei"
                font.pointSize: 11
                color: "#2E7D32"
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                visible: currentTranscript !== ""
            }
        }

        // 底部控制区域
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            color: "#FFFFFF"
            radius: 24

            // 顶部圆角阴影
            Rectangle {
                width: parent.width
                height: 24
                anchors.top: parent.top
                color: "transparent"

                Rectangle {
                    width: 40
                    height: 4
                    radius: 2
                    color: "#E0E0E0"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                // 状态文字
                Label {
                    id: statusLabel
                    Layout.alignment: Qt.AlignHCenter
                    text: {
                        if (isProcessing) return "正在处理..."
                        if (isRecording) return "正在聆听..."
                        return "点击麦克风开始说话"
                    }
                    font.family: "Microsoft YaHei"
                    font.pointSize: 14
                    font.weight: Font.Medium
                    color: isRecording ? "#007AFF" : "#666666"

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }

                // 录音按钮区域
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // 录音波纹动画
                    Item {
                        id: waveContainer
                        anchors.centerIn: parent
                        width: 160
                        height: 160

                        // 外层波纹
                        Repeater {
                            model: 3
                            Rectangle {
                                id: waveRing
                                property int ringIndex: index
                                anchors.centerIn: parent
                                width: 80
                                height: 80
                                radius: width / 2
                                color: "transparent"
                                border.color: "#007AFF"
                                border.width: 2
                                opacity: 0
                                visible: isRecording

                                SequentialAnimation on opacity {
                                    running: isRecording
                                    loops: Animation.Infinite
                                    PauseAnimation { duration: ringIndex * 400 }
                                    NumberAnimation { from: 0.6; to: 0; duration: 1200 }
                                }

                                SequentialAnimation on scale {
                                    running: isRecording
                                    loops: Animation.Infinite
                                    PauseAnimation { duration: ringIndex * 400 }
                                    NumberAnimation { from: 1; to: 2.5; duration: 1200 }
                                }
                            }
                        }

                        // 麦克风按钮
                        Rectangle {
                            id: micButton
                            anchors.centerIn: parent
                            width: 80
                            height: 80
                            radius: 40
                            color: isRecording ? "#FF3B30" : "#007AFF"

                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }

                            Behavior on scale {
                                NumberAnimation { duration: 100 }
                            }

                            // 麦克风图标
                            Image {
                                id: micIcon
                                anchors.centerIn: parent
                                width: 32
                                height: 32
                                source: isRecording ? "qrc:/img/icon/recording-01.svg" : "qrc:/img/icon/microphone-01.svg"
                                fillMode: Image.PreserveAspectFit

                                // 录音时的脉冲动画
                                SequentialAnimation on scale {
                                    running: isRecording
                                    loops: Animation.Infinite
                                    NumberAnimation { from: 1.0; to: 1.2; duration: 500; easing.type: Easing.InOutQuad }
                                    NumberAnimation { from: 1.2; to: 1.0; duration: 500; easing.type: Easing.InOutQuad }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true

                                onPressed: {
                                    micButton.scale = 0.95
                                }

                                onReleased: {
                                    micButton.scale = 1.0
                                }

                                onClicked: {
                                    if (isProcessing) return

                                    if (isRecording) {
                                        // 停止录音
                                        stopRecording()
                                    } else {
                                        // 开始录音
                                        startRecording()
                                    }
                                }
                            }

                            // 按钮阴影
                            layer.enabled: true
                        }
                    }

                    // 音频波形动画（录音时显示）
                    Row {
                        id: audioWaveform
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: waveContainer.top
                        anchors.bottomMargin: -30
                        spacing: 4
                        visible: isRecording
                        opacity: isRecording ? 1 : 0

                        Behavior on opacity {
                            NumberAnimation { duration: 200 }
                        }

                        Repeater {
                            model: 7
                            Rectangle {
                                id: waveBar
                                property int barIndex: index
                                width: 4
                                height: 20
                                radius: 2
                                color: "#007AFF"
                                anchors.verticalCenter: parent.verticalCenter

                                SequentialAnimation on height {
                                    running: isRecording
                                    loops: Animation.Infinite
                                    NumberAnimation {
                                        from: 8
                                        to: 8 + Math.random() * 32
                                        duration: 150 + barIndex * 50
                                        easing.type: Easing.InOutQuad
                                    }
                                    NumberAnimation {
                                        from: 8 + Math.random() * 32
                                        to: 8
                                        duration: 150 + barIndex * 50
                                        easing.type: Easing.InOutQuad
                                    }
                                }
                            }
                        }
                    }
                }

                // 底部操作按钮
                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 40

                    // 取消按钮（录音时显示）
                    RoundButton {
                        id: cancelButton
                        width: 50
                        height: 50
                        visible: isRecording
                        opacity: isRecording ? 1 : 0
                        icon.source: "qrc:/img/icon/x-close.svg"
                        icon.width: 20
                        icon.height: 20

                        background: Rectangle {
                            radius: 25
                            color: cancelButton.pressed ? "#E0E0E0" : "#F5F5F5"
                        }

                        Behavior on opacity {
                            NumberAnimation { duration: 200 }
                        }

                        onClicked: {
                            cancelRecording()
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "取消录音"
                    }

                    Item { Layout.fillWidth: true }

                    // 发送按钮（录音时显示）
                    RoundButton {
                        id: sendVoiceButton
                        width: 50
                        height: 50
                        visible: isRecording
                        opacity: isRecording ? 1 : 0
                        icon.source: "qrc:/img/icon/check.svg"
                        icon.width: 20
                        icon.height: 20

                        background: Rectangle {
                            radius: 25
                            color: sendVoiceButton.pressed ? "#45A049" : "#4CAF50"
                        }

                        Behavior on opacity {
                            NumberAnimation { duration: 200 }
                        }

                        onClicked: {
                            stopRecording()
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "发送语音"
                    }
                }
            }
        }
    }

    // 消息模型
    ListModel {
        id: voiceChatMessageModel
    }

    // 开始录音函数
    function startRecording() {
        isRecording = true
        currentTranscript = ""
        // 调用C++层开始录音
        app.startVoiceRecord()
    }

    // 停止录音函数
    function stopRecording() {
        isRecording = false
        isProcessing = true
        // 调用C++层停止录音
        app.stopVoiceRecord()
    }

    // 取消录音函数
    function cancelRecording() {
        isRecording = false
        currentTranscript = ""
        // 调用C++层停止录音但不发送
        app.stopVoiceRecord()
    }

    // 添加用户消息
    function addUserMessage(text) {
        if (text.trim() === "") return

        var timestamp = Qt.formatDateTime(new Date(), "hh:mm")
        voiceChatMessageModel.append({
            "context": text,
            "role": "User",
            "timestamp": timestamp
        })
    }

    // 添加AI回复消息
    function addAssistantMessage(text) {
        if (text.trim() === "") return

        var timestamp = Qt.formatDateTime(new Date(), "hh:mm")
        voiceChatMessageModel.append({
            "context": text,
            "role": "Assistant",
            "timestamp": timestamp
        })
    }

    // 连接Application信号
    Connections {
        target: app

        // 接收ASR实时转写
        function onSignal_asr_text(text, isFinal) {
            if (isFinal) {
                // 最终结果，添加用户消息
                if (text.trim() !== "") {
                    addUserMessage(text)
                }
                currentTranscript = ""
            } else {
                // 实时转写更新
                currentTranscript = text
            }
        }

        // 接收LLM回复
        function onSignal_receive_llm(content) {
            isProcessing = false
            addAssistantMessage(content)
        }
    }

    // 模拟ASR实时转写（实际应连接到后端信号）
    Timer {
        id: transcriptSimulator
        interval: 100
        running: false
        repeat: true
        onTriggered: {
            // 这里应该从后端获取实时转写结果
            // 目前仅作为占位符
        }
    }

    // 组件加载完成
    Component.onCompleted: {
        console.log("VoiceChatPage loaded")
    }
}
