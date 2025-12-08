import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "../Components"
import "../js/addListModel.js" as AddListModel

Page {
    id: voiceChatPage

    // 录音状态
    property bool isRecording: false
    property bool isProcessing: false  // LLM处理中
    property bool isPlayingTts: false  // TTS播放中
    property bool canRecord: true      // 是否允许录音
    property string currentTranscript: ""

    signal signal_sendVoiceMessage(string text)
    
    // 音频播放器用于试听
    MediaPlayer {
        id: previewPlayer
        audioOutput: AudioOutput {}
    }

    background: Rectangle {
        color: "#f5f5f5"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 顶部工具栏：语音选择
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            color: "#FFFFFF"
            
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 12
                
                Label {
                    text: "语音角色："
                    font.family: "Microsoft YaHei"
                    font.pointSize: 11
                    color: "#666666"
                }
                
                ComboBox {
                    id: voiceSampleComboBox
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 36
                    
                    model: ListModel {
                        id: voiceSampleModel
                        ListElement { text: "灰原哀 样本1"; value: ":/sample/huiyuanai.WAV" }
                        ListElement { text: "灰原哀 样本2"; value: ":/sample/huiyuanai2.WAV" }
                        ListElement { text: "自定义样本"; value: "custom" }
                    }
                    
                    textRole: "text"
                    
                    // 更新自定义样本显示名称
                    function updateCustomName() {
                        var customName = configMgr.customVoiceName
                        if (customName && customName !== "") {
                            voiceSampleModel.setProperty(2, "text", customName)
                        }
                    }
                    
                    // 同步配置
                    function syncFromConfig() {
                        updateCustomName()
                        // 从配置加载当前选中的语音样本索引
                        if (configMgr.currentVoiceSample === -1) {
                            currentIndex = 2
                        } else if (configMgr.currentVoiceSample >= 0 && configMgr.currentVoiceSample < 2) {
                            currentIndex = configMgr.currentVoiceSample
                        }
                    }
                    
                    Component.onCompleted: {
                        syncFromConfig()
                    }
                    
                    Connections {
                        target: configMgr
                        function onCustomVoiceNameChanged() {
                            voiceSampleComboBox.updateCustomName()
                        }
                        function onCurrentVoiceSampleChanged() {
                            voiceSampleComboBox.syncFromConfig()
                        }
                    }
                    
                    onCurrentIndexChanged: {
                        if (currentIndex >= 0 && currentIndex < voiceSampleModel.count) {
                            if (currentIndex === 0) {
                                configMgr.currentVoiceSample = 0
                            } else if (currentIndex === 1) {
                                configMgr.currentVoiceSample = 1
                            } else if (currentIndex === 2) {
                                configMgr.currentVoiceSample = -1
                            }
                            // 应用当前语音样本配置
                            configMgr.applyCurrentVoiceSample()
                        }
                    }
                    
                    background: Rectangle {
                        radius: 8
                        color: voiceSampleComboBox.down ? "#f0f0f0" : "#f5f5f5"
                        border.color: voiceSampleComboBox.activeFocus ? "#007AFF" : "#e0e0e0"
                        border.width: 1
                        
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                }
                
                // 试听按钮
                Button {
                    id: previewVoiceBtn
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 36
                    text: "试听"
                    
                    contentItem: Label {
                        text: parent.text
                        font.family: "Microsoft YaHei"
                        font.pointSize: 10
                        color: "#007AFF"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    background: Rectangle {
                        radius: 8
                        color: previewVoiceBtn.down ? "#e6f3ff" : (previewVoiceBtn.hovered ? "#f0f8ff" : "transparent")
                        border.color: "#007AFF"
                        border.width: 1
                        
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                    
                    onClicked: {
                        var wavPath = ""
                        if (voiceSampleComboBox.currentIndex === 0) {
                            wavPath = "qrc:/sample/huiyuanai.WAV"
                        } else if (voiceSampleComboBox.currentIndex === 1) {
                            wavPath = "qrc:/sample/huiyuanai2.WAV"
                        } else if (voiceSampleComboBox.currentIndex === 2) {
                            // 自定义样本：使用 configMgr 中保存的路径
                            var customPath = configMgr.customVoiceWav
                            console.log("自定义样本路径:", customPath)
                            // 检查是否是有效的本地文件路径（包含盘符如 D: 或 C:）
                            if (customPath && customPath.length > 3 && customPath.charAt(1) === ':') {
                                wavPath = "file:///" + customPath
                            } else if (customPath && customPath.startsWith(":/")) {
                                // 资源文件路径 :/ 转换为 qrc:/
                                wavPath = "qrc" + customPath.substring(1)
                            } else {
                                console.log("自定义样本路径为空或无效，请在设置页面选择文件")
                                return
                            }
                        }
                        
                        if (wavPath !== "") {
                            console.log("播放:", wavPath)
                            previewPlayer.source = wavPath
                            previewPlayer.play()
                        }
                    }
                    
                    ToolTip.visible: hovered
                    ToolTip.text: "试听当前语音角色"
                }
                
                Item { Layout.fillWidth: true }
                
                // 当前语音角色显示
                Row {
                    spacing: 6
                    
                    Rectangle {
                        width: 8
                        height: 8
                        radius: 4
                        color: "#4CAF50"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Label {
                        text: voiceSampleComboBox.currentText
                        font.family: "Microsoft YaHei"
                        font.pointSize: 10
                        font.bold: true
                        color: "#333"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            
            // 底部分隔线
            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: "#E8E8E8"
            }
        }

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
                        text: model.role === "User" ? "你" : configMgr.aiName
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

            // "正在输入"动画footer
            footer: Item {
                width: chatListView.width
                height: (isProcessing || isPlayingTts) ? 60 : 0
                visible: isProcessing || isPlayingTts

                Behavior on height {
                    NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
                }

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Label {
                        text: isPlayingTts ? (configMgr.aiName + " 正在说话") : (configMgr.aiName + " 正在输入")
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 13
                        color: "#666666"
                    }

                    // 打字动画点
                    Row {
                        spacing: 4
                        anchors.verticalCenter: parent.verticalCenter

                        Repeater {
                            model: 3
                            Rectangle {
                                width: 6
                                height: 6
                                radius: 3
                                color: "#007AFF"

                                SequentialAnimation on opacity {
                                    running: isProcessing || isPlayingTts
                                    loops: Animation.Infinite
                                    PauseAnimation { duration: index * 200 }
                                    NumberAnimation { from: 0.3; to: 1; duration: 400 }
                                    NumberAnimation { from: 1; to: 0.3; duration: 400 }
                                }

                                SequentialAnimation on y {
                                    running: isProcessing || isPlayingTts
                                    loops: Animation.Infinite
                                    PauseAnimation { duration: index * 200 }
                                    NumberAnimation { from: 0; to: -4; duration: 300; easing.type: Easing.OutQuad }
                                    NumberAnimation { from: -4; to: 0; duration: 300; easing.type: Easing.InQuad }
                                }
                            }
                        }
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
                        if (isPlayingTts) return "正在播放回复..."
                        if (isProcessing) return "正在思考..."
                        if (isRecording) return "正在聆听..."
                        if (!canRecord) return "请等待回复完成..."
                        return "点击麦克风开始说话"
                    }
                    font.family: "Microsoft YaHei"
                    font.pointSize: 14
                    font.weight: Font.Medium
                    color: isRecording ? "#007AFF" : (isProcessing || isPlayingTts ? "#FF9800" : "#666666")

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
                                    // 如果正在处理或播放TTS，不允许操作
                                    if (!canRecord || isProcessing || isPlayingTts) return

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
                            color:  "#007AFF"
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

    // 服务器状态警告对话框
    Dialog {
        id: serverStatusDialog
        title: "服务未连接"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        
        Label {
            text: "检测到部分服务未连接，请检查服务器状态。\n\n" +
                  "Ollama: " + (app.ollamaOnline ? "在线" : "离线") + "\n" +
                  "ASR: " + (app.asrOnline ? "在线" : "离线") + "\n" +
                  "TTS: " + (app.ttsOnline ? "在线" : "离线")
            font.family: "Microsoft YaHei"
        }
    }

    // 消息模型
    ListModel {
        id: voiceChatMessageModel
    }

    // 开始录音函数
    function startRecording() {
        // 检查服务器状态
        if (!app.ollamaOnline || !app.asrOnline || !app.ttsOnline) {
            serverStatusDialog.open()
            return
        }

        if (!canRecord) return
        isRecording = true
        currentTranscript = ""
        // 调用C++层开始录音
        app.startVoiceRecord()
    }

    // 停止录音函数
    function stopRecording() {
        isRecording = false
        isProcessing = true
        canRecord = false  // 禁止再次录音，直到回复完成
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

    // 自动开始录音（TTS播放完成后调用）
    function autoStartRecording() {
        canRecord = true
        // 延迟一小段时间后自动开始录音
        autoRecordTimer.start()
    }

    // 自动录音定时器
    Timer {
        id: autoRecordTimer
        interval: 500  // 延迟500ms
        repeat: false
        onTriggered: {
            if (canRecord && !isProcessing && !isPlayingTts) {
                startRecording()
            }
        }
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
                // 最终结果
                currentTranscript = ""
                if (text.trim() !== "") {
                    // 有内容，添加用户消息，等待LLM回复
                    addUserMessage(text)
                } else {
                    // 空消息，用户未说话，立即恢复可录音状态
                    console.log("用户未说话，取消本次录音")
                    isProcessing = false
                    canRecord = true
                }
            } else {
                // 实时转写更新
                currentTranscript = text
            }
        }

        // 收到停止录音信号（ASR返回offline结果后）
        function onSignal_stop_recording() {
            if (isRecording) {
                console.log("收到停止录音信号，自动停止录音")
                isRecording = false
                isProcessing = true
                canRecord = false
                // 调用C++层停止录音
                app.stopVoiceRecord()
            }
        }

        // 接收LLM回复
        function onSignal_receive_llm(content) {
            isProcessing = false
            isPlayingTts = true  // 开始播放TTS
            addAssistantMessage(content)
        }
        
        // TTS播放完成
        function onSignal_tts_playback_finished() {
            isPlayingTts = false
            // 自动开始下一轮录音
            autoStartRecording()
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
        // 语音对话页面不加载历史消息，每次进入是新的语音会话
        // 如果需要加载历史，应该有独立的语音对话历史
    }
}
