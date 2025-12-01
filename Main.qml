import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components"

ApplicationWindow{
    id:window
    width: 1024
    height: 800
    visible: true
    title: qsTr("你的女孩")
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
            width:parent.width
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
            }


        }
    }


}
