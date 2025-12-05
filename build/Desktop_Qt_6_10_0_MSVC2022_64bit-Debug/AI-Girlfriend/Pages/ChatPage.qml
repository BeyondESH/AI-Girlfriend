import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../Components"

Page {
    id: root
    
    signal voiceModeRequested()

    background: Rectangle { color: "#f0f0f4" }

    ListModel {
        id: chatModel
        ListElement { text: "你好，我是你的AI助手。"; isUser: false }
        ListElement { text: "今天天气怎么样？"; isUser: true }
        ListElement { text: "今天天气不错，适合出去走走。"; isUser: false }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ListView {
            id: chatListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: chatModel
            spacing: 10
            displayMarginBeginning: 40
            displayMarginEnd: 40
            
            // Add some padding at the top and bottom
            header: Item { height: 20 }
            footer: Item { height: 20 }

            delegate: MessageBubble {
                messageText: model.text
                isUser: model.isUser
            }
            
            // Auto-scroll to bottom
            onCountChanged: {
                Qt.callLater(function() {
                    chatListView.positionViewAtEnd()
                })
            }
        }

        // Input Area
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: Math.min(parent.width - 40, 750)
            Layout.bottomMargin: 20
            height: inputLayout.implicitHeight + 20
            color: "#f0f0f4"
            radius: 20
            
            RowLayout {
                id: inputLayout
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                ScrollView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Math.min(Math.max(messageInput.contentHeight, 40), 150)
                    
                    TextArea {
                        id: messageInput
                        placeholderText: qsTr("发送信息/语音对话")
                        wrapMode: TextEdit.Wrap
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 14
                        selectByMouse: true
                        verticalAlignment: TextEdit.AlignVCenter
                        
                        background: Rectangle {
                            color: "transparent"
                        }
                    }
                }

                Switch {
                    text: "深度思考"
                    font.family: "Microsoft YaHei"
                    visible: parent.width > 500 // Hide on very small screens
                }

                RoundButton {
                    id: micButton
                    icon.source: "qrc:/img/icon/microphone-01.svg"
                    flat: true
                    onClicked: root.voiceModeRequested()
                }

                RoundButton {
                    id: sendButton
                    icon.source: "qrc:/img/icon/arrow-narrow-right.svg" 
                    flat: true
                    onClicked: {
                        if (messageInput.text.trim().length > 0) {
                            chatModel.append({
                                "text": messageInput.text,
                                "isUser": true
                            })
                            Qt.callLater(function() {
                                chatModel.append({
                                    "text": "收到: " + messageInput.text,
                                    "isUser": false
                                })
                            })
                            messageInput.text = ""
                        }
                    }
                }
            }
        }
    }
}
