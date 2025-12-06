import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia

Page {
    id: settingPage
    
    // 音频播放器用于试听
    MediaPlayer {
        id: previewPlayer
        audioOutput: AudioOutput {}
    }
    
    // 文件选择对话框
    FileDialog {
        id: voiceSampleFileDialog
        title: "选择语音样本文件"
        nameFilters: ["音频文件 (*.wav *.WAV *.mp3 *.MP3)"]
        onAccepted: {
            var filePath = selectedFile.toString().replace("file:///", "")
            customWavPathField.text = filePath
            configMgr.ttsPromptWav = filePath
            configMgr.currentVoiceSample = -1 // 使用自定义样本
            voiceSampleCombo.currentIndex = 2
        }
    }
    
    background: Rectangle {
        color: "#f5f5f5"
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 20
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 20

            // 页面标题
            Label {
                text: "个性化定制"
                font.family: "Microsoft YaHei"
                font.pixelSize: 28
                font.bold: true
                color: "#333333"
                Layout.bottomMargin: 10
            }

            // 用户设置卡片
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: userColumn.implicitHeight + 40
                radius: 12
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                border.width: 1

                ColumnLayout {
                    id: userColumn
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 16

                    Label {
                        text: "用户设置"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 20

                        Label {
                            text: "你的名字"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                            Layout.preferredWidth: 100
                        }

                        TextField {
                            id: userNameField
                            Layout.fillWidth: true
                            placeholderText: "输入你的名字"
                            font.family: "Microsoft YaHei"
                            text: configMgr.userName
                            onTextChanged: configMgr.userName = text
                        }
                    }

                    Label {
                        text: "关于你（可选）"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    TextArea {
                        id: userDescField
                        Layout.fillWidth: true
                        Layout.preferredHeight: 80
                        placeholderText: "介绍一下你自己，让AI更了解你..."
                        font.family: "Microsoft YaHei"
                        wrapMode: TextArea.Wrap
                        text: configMgr.userDescription
                        onTextChanged: configMgr.userDescription = text
                        background: Rectangle {
                            color: "#F5F5F5"
                            radius: 8
                            border.color: userDescField.activeFocus ? "#007AFF" : "#E0E0E0"
                        }
                    }
                }
            }

            // AI角色设置卡片
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: aiColumn.implicitHeight + 40
                radius: 12
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                border.width: 1

                ColumnLayout {
                    id: aiColumn
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 16

                    Label {
                        text: "AI角色设置"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 20

                        Label {
                            text: "AI名字"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                            Layout.preferredWidth: 100
                        }

                        TextField {
                            id: aiNameField
                            Layout.fillWidth: true
                            placeholderText: "给AI起个名字"
                            font.family: "Microsoft YaHei"
                            text: configMgr.aiName
                            onTextChanged: configMgr.aiName = text
                        }
                    }

                    Label {
                        text: "AI性格描述"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    TextArea {
                        id: aiPersonalityField
                        Layout.fillWidth: true
                        Layout.preferredHeight: 100
                        placeholderText: "描述AI的性格特点、说话风格等..."
                        font.family: "Microsoft YaHei"
                        wrapMode: TextArea.Wrap
                        text: configMgr.aiPersonality
                        onTextChanged: configMgr.aiPersonality = text
                        background: Rectangle {
                            color: "#F5F5F5"
                            radius: 8
                            border.color: aiPersonalityField.activeFocus ? "#007AFF" : "#E0E0E0"
                        }
                    }

                    Label {
                        text: "自定义系统提示词（高级）"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    TextArea {
                        id: systemPromptField
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                        placeholderText: "留空则使用默认提示词模板。自定义提示词将完全替代默认模板..."
                        font.family: "Microsoft YaHei"
                        wrapMode: TextArea.Wrap
                        text: configMgr.systemPrompt
                        onTextChanged: configMgr.systemPrompt = text
                        background: Rectangle {
                            color: "#F5F5F5"
                            radius: 8
                            border.color: systemPromptField.activeFocus ? "#007AFF" : "#E0E0E0"
                        }
                    }
                }
            }

            // 服务器配置卡片
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: serverColumn.implicitHeight + 40
                radius: 12
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                border.width: 1

                ColumnLayout {
                    id: serverColumn
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 16

                    Label {
                        text: "服务器配置"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 20
                        rowSpacing: 12

                        Label {
                            text: "LLM服务器"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                        }
                        TextField {
                            id: llmUrlField
                            Layout.fillWidth: true
                            placeholderText: "http://localhost:11434/api/chat"
                            font.family: "Microsoft YaHei"
                            text: configMgr.llmServerUrl
                            onTextChanged: configMgr.llmServerUrl = text
                        }

                        Label {
                            text: "LLM模型"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                        }
                        TextField {
                            id: llmModelField
                            Layout.fillWidth: true
                            placeholderText: "qwen3:8b"
                            font.family: "Microsoft YaHei"
                            text: configMgr.llmModelName
                            onTextChanged: configMgr.llmModelName = text
                        }

                        Label {
                            text: "ASR服务器"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                        }
                        TextField {
                            id: asrUrlField
                            Layout.fillWidth: true
                            placeholderText: "ws://localhost:10096"
                            font.family: "Microsoft YaHei"
                            text: configMgr.asrServerUrl
                            onTextChanged: configMgr.asrServerUrl = text
                        }

                        Label {
                            text: "TTS服务器"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                        }
                        TextField {
                            id: ttsUrlField
                            Layout.fillWidth: true
                            placeholderText: "http://localhost:50000/inference_zero_shot"
                            font.family: "Microsoft YaHei"
                            text: configMgr.ttsServerUrl
                            onTextChanged: configMgr.ttsServerUrl = text
                        }
                    }
                }
            }

            // 语音克隆设置卡片
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: voiceColumn.implicitHeight + 40
                radius: 12
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                border.width: 1

                ColumnLayout {
                    id: voiceColumn
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 16

                    Label {
                        text: "语音克隆设置"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                    }

                    // 预设语音样本选择
                    Label {
                        text: "预设语音样本"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        ComboBox {
                            id: voiceSampleCombo
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            model: ["灰原哀 样本1", "灰原哀 样本2", "自定义样本"]
                            currentIndex: configMgr.currentVoiceSample >= 0 ? configMgr.currentVoiceSample : 2
                            
                            onCurrentIndexChanged: {
                                if (currentIndex === 0) {
                                    configMgr.currentVoiceSample = 0
                                    configMgr.ttsPromptWav = ":/sample/huiyuanai.WAV"
                                    configMgr.ttsPromptText = "这个时候，应该早就闯进了博士家才对，但是刚才博士传来的简讯，都是在说今天晚餐的事"
                                } else if (currentIndex === 1) {
                                    configMgr.currentVoiceSample = 1
                                    configMgr.ttsPromptWav = ":/sample/huiyuanai2.WAV"
                                    configMgr.ttsPromptText = "不过，应该没事吧，如果那个姓黑田的人，真的就是你所怀疑的朗姆，又在那么近的距离看到我这张脸，照理说应该会察觉我就是背叛组织的雪莉，这个时候，应该早就闯进了博士家才对，但是刚才博士传来的简讯，都是在说今天晚餐的事"
                                } else {
                                    configMgr.currentVoiceSample = -1
                                }
                            }
                        }

                        Button {
                            text: "试听"
                            font.family: "Microsoft YaHei"
                            Layout.preferredWidth: 80
                            enabled: voiceSampleCombo.currentIndex < 2 || (voiceSampleCombo.currentIndex === 2 && customWavPathField.text !== "")
                            onClicked: {
                                var wavPath = ""
                                if (voiceSampleCombo.currentIndex === 0) {
                                    wavPath = "qrc:/sample/huiyuanai.WAV"
                                } else if (voiceSampleCombo.currentIndex === 1) {
                                    wavPath = "qrc:/sample/huiyuanai2.WAV"
                                } else if (voiceSampleCombo.currentIndex === 2 && customWavPathField.text !== "") {
                                    wavPath = "file:///" + customWavPathField.text
                                }
                                if (wavPath !== "") {
                                    previewPlayer.source = wavPath
                                    previewPlayer.play()
                                }
                            }
                        }
                    }

                    // 自定义样本名称
                    Label {
                        text: "自定义样本名称"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#666666"
                        visible: voiceSampleCombo.currentIndex === 2
                    }

                    TextField {
                        id: customVoiceNameField
                        Layout.fillWidth: true
                        placeholderText: "输入自定义语音样本的名称（如：我的声音）"
                        font.family: "Microsoft YaHei"
                        text: configMgr.customVoiceName
                        visible: voiceSampleCombo.currentIndex === 2
                        onTextChanged: configMgr.customVoiceName = text
                    }

                    // 自定义样本文件选择
                    Label {
                        text: "自定义样本文件"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        color: "#666666"
                        visible: voiceSampleCombo.currentIndex === 2
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        visible: voiceSampleCombo.currentIndex === 2

                        TextField {
                            id: customWavPathField
                            Layout.fillWidth: true
                            placeholderText: "点击浏览选择音频文件 (WAV/MP3)"
                            font.family: "Microsoft YaHei"
                            readOnly: true
                            text: configMgr.currentVoiceSample === -1 ? configMgr.ttsPromptWav : ""
                        }

                        Button {
                            text: "浏览"
                            font.family: "Microsoft YaHei"
                            onClicked: voiceSampleFileDialog.open()
                        }
                        
                        Button {
                            text: "试听"
                            font.family: "Microsoft YaHei"
                            enabled: customWavPathField.text !== ""
                            onClicked: {
                                if (customWavPathField.text !== "") {
                                    previewPlayer.source = "file:///" + customWavPathField.text
                                    previewPlayer.play()
                                }
                            }
                        }
                    }

                    // 参考文本与自动识别
                    RowLayout {
                        Layout.fillWidth: true
                        visible: voiceSampleCombo.currentIndex === 2
                        
                        Label {
                            text: "参考文本（与音频内容一致）"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            color: "#666666"
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        Button {
                            text: app.audioRecognizeStatus === "正在识别..." ? "识别中..." : "自动识别"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 12
                            enabled: customWavPathField.text !== "" && app.audioRecognizeStatus !== "正在识别..."
                            onClicked: {
                                if (customWavPathField.text !== "") {
                                    app.recognizeAudioFile(customWavPathField.text)
                                }
                            }
                            
                            ToolTip.visible: hovered
                            ToolTip.text: "使用语音识别自动填充文本内容"
                        }
                    }

                    TextArea {
                        id: ttsPromptTextField
                        Layout.fillWidth: true
                        Layout.preferredHeight: 100
                        placeholderText: "输入与语音样本对应的文字内容，或点击自动识别..."
                        font.family: "Microsoft YaHei"
                        wrapMode: TextArea.Wrap
                        text: configMgr.ttsPromptText
                        onTextChanged: configMgr.ttsPromptText = text
                        visible: voiceSampleCombo.currentIndex === 2
                        background: Rectangle {
                            color: "#F5F5F5"
                            radius: 8
                            border.color: ttsPromptTextField.activeFocus ? "#007AFF" : "#E0E0E0"
                        }
                    }
                    
                    // 识别状态显示
                    Label {
                        text: app.audioRecognizeStatus
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 11
                        color: app.audioRecognizeStatus === "识别完成" ? "#4CAF50" : 
                               app.audioRecognizeStatus === "正在识别..." ? "#FF9800" : "#666666"
                        visible: voiceSampleCombo.currentIndex === 2 && app.audioRecognizeStatus !== "就绪"
                    }

                    // 提示信息
                    Label {
                        text: "提示：语音克隆需要提供一段参考音频和对应的文字，系统会学习该音色来合成新的语音"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 11
                        color: "#999999"
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }
                }
            }

            // 操作按钮
            RowLayout {
                Layout.fillWidth: true
                Layout.topMargin: 10
                spacing: 20

                Item { Layout.fillWidth: true }

                Button {
                    text: "重置为默认"
                    font.family: "Microsoft YaHei"
                    onClicked: {
                        resetDialog.open()
                    }
                }

                Button {
                    text: "保存设置"
                    font.family: "Microsoft YaHei"
                    highlighted: true
                    onClicked: {
                        configMgr.saveConfig()
                        saveSuccessAnimation.start()
                    }
                }
            }

            // 保存成功提示
            Label {
                id: saveSuccessLabel
                text: "✓ 设置已保存"
                font.family: "Microsoft YaHei"
                font.pixelSize: 14
                color: "#4CAF50"
                opacity: 0
                Layout.alignment: Qt.AlignHCenter

                SequentialAnimation {
                    id: saveSuccessAnimation
                    NumberAnimation { target: saveSuccessLabel; property: "opacity"; to: 1; duration: 200 }
                    PauseAnimation { duration: 2000 }
                    NumberAnimation { target: saveSuccessLabel; property: "opacity"; to: 0; duration: 500 }
                }
            }

            Item { Layout.preferredHeight: 40 }
        }
    }

    // 重置确认对话框
    Dialog {
        id: resetDialog
        title: "确认重置"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Yes | Dialog.No

        Label {
            text: "确定要将所有设置重置为默认值吗？"
            font.family: "Microsoft YaHei"
        }

        onAccepted: {
            configMgr.resetToDefault()
        }
    }
}
