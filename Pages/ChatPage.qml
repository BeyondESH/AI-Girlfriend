import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../Components"
import "../js/addListModel.js" as AddListModel

Page{
    // anchors.fill:parent
    id:page
    signal signal_sendMessage(string text);
    ColumnLayout{
        anchors.fill:parent
        spacing: 0
        ListView{
            id:listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            visible: false
            model: ChatMessageListModel{}
            spacing: 20
            clip: true
            delegate:Column{
                id:column
                width: listView.width
                Label{
                    id:nameLabel
                    text:"雅萱"
                    font.family: "Microsoft YaHei"
                    font.pointSize: 8
                    visible: true
                    color:"#4b4b4b"
                    anchors.left: parent.left
                    anchors.leftMargin:30
                }
                Rectangle{
                    id:messageRect
                    width: messageLabel.width+20
                    height:messageLabel.implicitHeight+20
                    property string role:model.role
                    color:role==="User"?"#A9E87A":"#FFFFFF"
                    Label{
                        id:messageLabel
                        anchors.centerIn: parent
                        font.family: "Microsoft YaHei"
                        font.pointSize: 12
                        text:model.context
                        wrapMode: Text.Wrap
                        clip: true
                        width: Math.min(implicitWidth, listView.width * 0.5)
                    }

                    Component.onCompleted: {
                        if(role==="User"){
                            anchors.right= parent.right
                            radius=10
                            anchors.rightMargin=20
                            nameLabel.visible=false
                        }else if(role==="Assistant"){
                            anchors.left=parent.left
                            anchors.leftMargin=20
                        }else{
                            column.visible=false
                        }
                    }
                }
            }

        }
        Column{
            Layout.alignment: Qt.AlignCenter
            bottomPadding: 20
            spacing:50
            Label{
                id:greetLabel
                visible: true
                anchors.horizontalCenter: parent.horizontalCenter
                text:"你好，我能为你做点什么"
                font.family: "Microsoft YaHei"
                font.pixelSize: 25
            }
            Rectangle{
                radius: 20
                anchors.horizontalCenter: parent.horizontalCenter
                width:750
                height: chatColumn.implicitHeight
                color:"#f0f0f4"
                Column{
                    id:chatColumn
                    anchors.fill:parent
                    spacing:10
                    bottomPadding: 5
                    topPadding: 5
                    ScrollView{
                        id:sendScrollView
                        width:parent.width
                        height:textArea.contentHeight+30
                        clip: true
                        TextArea{
                            id: textArea
                            leftPadding: 20
                            topPadding: 20
                            width:parent.width
                            height:100
                            placeholderText: "发送信息/语音对话"
                            font.family: "Microsoft YaHei"
                            font.pointSize: 12
                            wrapMode:TextEdit.Wrap
                            background: Rectangle {
                                color: "#ededed"
                                radius: 4
                            }

                            onLengthChanged: {
                                if(length===0){
                                    sendButton.enabled=false
                                }else{
                                    sendButton.enabled=true
                                }
                            }

                            property int maxHeight: 200
                            onContentHeightChanged: {
                                if (contentHeight >= maxHeight) {
                                    sendScrollView.height = maxHeight
                                }else{
                                    sendScrollView.height=textArea.contentHeight+30
                                }
                            }


                        }
                    }
                    RowLayout{
                        anchors.horizontalCenter: parent.horizontalCenter
                        width:parent.width
                        spacing:15

                        Switch{
                            Layout.leftMargin: 10
                            text:"深度思考"
                            font.family: "Microsoft YaHei"
                        }
                        Item{
                            Layout.fillWidth: true
                        }
                        RoundButton {
                            id:microphoneButton
                            icon.source: "qrc:/img/icon/microphone-01.svg"
                        }
                        RoundButton {
                            id:sendButton
                            Layout.rightMargin: 10
                            icon.source:"qrc:/img/icon/arrow-narrow-right.svg"
                            enabled:false
                            onClicked: {
                                greetLabel.visible=false;
                                listView.visible=true
                                AddListModel.addChatMessage(listView.model,textArea.text)
                                page.signal_sendMessage(textArea.text)
                                app.sendChatMessage(textArea.text)
                                textArea.clear()
                            }
                        }
                    }
                }
            }
        }
    }

    Connections{
        target: app
        function onSignal_receive_llm(content){
            AddListModel.addChatMessage(listView.model,content,"Assistant")
        }
    }
}
