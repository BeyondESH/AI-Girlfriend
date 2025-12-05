import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings 1.0

Page {
    id: root
    
    background: Rectangle { color: "#f0f0f4" }

    Settings {
        id: settings
        property string userName: "User"
        property string aiName: "AI Girlfriend"
        property string systemPrompt: "You are a helpful AI assistant."
        property string modelName: "Qwen3:8b"
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: parent.width
        
        ColumnLayout {
            width: Math.min(parent.width - 40, 800)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            
            Label {
                text: qsTr("个性化定制")
                font.family: "Microsoft YaHei"
                font.pixelSize: 24
                font.bold: true
                Layout.topMargin: 20
            }

            GroupBox {
                title: qsTr("基本设置")
                Layout.fillWidth: true
                font.family: "Microsoft YaHei"
                
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 15
                    
                    Label { text: qsTr("你的名字"); font.family: "Microsoft YaHei" }
                    TextField {
                        Layout.fillWidth: true
                        text: settings.userName
                        onEditingFinished: settings.userName = text
                        font.family: "Microsoft YaHei"
                    }

                    Label { text: qsTr("AI 名字"); font.family: "Microsoft YaHei" }
                    TextField {
                        Layout.fillWidth: true
                        text: settings.aiName
                        onEditingFinished: settings.aiName = text
                        font.family: "Microsoft YaHei"
                    }
                }
            }

            GroupBox {
                title: qsTr("模型设置")
                Layout.fillWidth: true
                font.family: "Microsoft YaHei"
                
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 15
                    
                    Label { text: qsTr("模型名称"); font.family: "Microsoft YaHei" }
                    ComboBox {
                        Layout.fillWidth: true
                        editable: true
                        model: ["Qwen3:8b", "Llama3.2", "Qwen3-vl:8b"]
                        currentIndex: model.indexOf(settings.modelName)
                        onAccepted: {
                            if (find(editText) === -1)
                                model.append(editText)
                            settings.modelName = editText
                        }
                        onActivated: settings.modelName = currentText
                        font.family: "Microsoft YaHei"
                    }

                    Label { text: qsTr("系统提示词 (System Prompt)"); font.family: "Microsoft YaHei" }
                    TextArea {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                        text: settings.systemPrompt
                        onEditingFinished: settings.systemPrompt = text
                        font.family: "Microsoft YaHei"
                        wrapMode: TextEdit.Wrap
                        background: Rectangle {
                            border.color: "#ccc"
                            radius: 4
                        }
                    }
                }
            }
        }
    }
}
