import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components"
// import "Pages"

ApplicationWindow{
    id:window
    width: 1024
    height: 800
    visible: true
    title: qsTr("你的女孩")
    minimumWidth:1024
    header:Rectangle{
        height: headerLayout.implicitHeight+10
        width:parent.width
        // color:"lightgray"
        RowLayout{
            id:headerLayout
            anchors.fill:parent
            spacing:10
            Button{
                id:drawOpenButton
                Layout.leftMargin: 15
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                padding: 0
                flat: true
                visible: true
                contentItem: Item {
                    anchors.fill: parent
                    Image {
                        source: "qrc:/img/icon/a-HeaderIcon.svg"
                        width: 20
                        height: 20
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("显示侧边栏")

                MouseArea{
                    anchors.fill:parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:{
                        if(drawer.position===0){
                            drawer.open()
                        }else{
                            drawer.close()
                        }
                    }
                }
            }
            Button{
                id:newChatButton
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                padding: 0
                flat: true
                visible: true
                contentItem: Item {
                    anchors.fill: parent
                    Image {
                        source: "qrc:/img/icon/message-plus-square.svg"
                        width: 20
                        height: 20
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("开启新对话")

                MouseArea{
                    anchors.fill:parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:{
                        print("进入新对话")
                    }
                }
            }
            ComboBox{
                Layout.preferredWidth: 150
                Layout.preferredHeight: 30
                editable: true
                flat:true
                model: ListModel {
                    id: model
                    ListElement { text: "Qwen3:8b" }
                    ListElement { text: "Llama3.2" }
                    ListElement { text: "Qwen3-vl:8b" }
                }
                onAccepted: {
                    if (find(editText) === -1)
                        model.append({text: editText})
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("选择大模型")
            }
            Item{
                Layout.fillWidth: true
            }
            Label{
                text:"by BeyondESH"
                font.family: "Microsoft YaHei"
                rightPadding: 30
                font.bold: true
            }
        }
    }

    Drawer {
        id:drawer
        y: header.height
        height: window.height - header.height
        width: window.width * 0.2
        // onOpened: {
        //     newChatButton1.visible=false
        // }

        // onClosed:{
        //     newChatButton1.visible=true
        // }
        ColumnLayout{
            anchors.fill: parent
            spacing:0

            SideButton{
                id:newChatSideButton
            }

            SideButton{
                id:settingSideButton
                text:qsTr("个性化定制")
                image:"qrc:/img/icon/user-02.svg"
            }

            SideButton{
                id:stateSideButton
                text:qsTr("服务器状态")
                image:"qrc:/img/icon/cehua.svg"
            }

            MenuSeparator {
                padding: 0
                topPadding: 12
                bottomPadding: 12
                contentItem: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 1
                    color: "#1E000000"
                }
            }

            Label{
                text: qsTr("历史对话")
                color: "#6c6c6c"
                font.family: "Microsoft YaHei"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.leftMargin: 20
                Layout.bottomMargin: 15
            }

            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                ListView{
                    id:historyListView
                    anchors.fill:parent
                    model:HistoryListModel{
                    }
                    spacing: 5
                    clip: true

                    delegate:Button{
                        id:delegateButton
                        width: parent.width
                        height:30
                        flat:true
                        Text{
                            width:parent.width
                            id:text
                            elide:Text.ElideRight
                            clip:true
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            leftPadding: 20
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            text:context
                            color:"#4b4b4b"
                        }
                        MouseArea{
                            anchors.fill:parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:{

                            }
                        }
                    }
                }
            }
        }
    }
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: ColumnLayout{
            anchors.fill:parent
            spacing: 0

            Label{
                id:greetLabel
                visible: true
                Layout.alignment: Qt.AlignHCenter
                text:"你好，我能为你做点什么"
                font.family: "Microsoft YaHei"
                font.pixelSize: 25
            }
            Rectangle{
                radius: 20
                Layout.alignment: Qt.AlignHCenter
                width:750
                height: chatColumn.implicitHeight
                color:"#f0f0f4"
                Column{
                    id:chatColumn
                    anchors.fill:parent
                    spacing:10
                    bottomPadding: 5
                    topPadding: 5
                    TextArea{
                        width:parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        placeholderText: "发送信息/语音对话"
                        font.family: "Microsoft YaHei"
                        background: Rectangle {
                            color: "#f0f0f4"  // 与你的背景色一致
                            radius: 4         // 可选，圆角效果
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
                            icon.source: "qrc:/img/icon/microphone-01.svg"
                        }
                        RoundButton {
                            Layout.rightMargin: 10
                            text:"发送"
                        }
                    }
                }
            }
        }
    }
}
