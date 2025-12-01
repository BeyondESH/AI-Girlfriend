import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


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
                Layout.leftMargin: 25
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
                onClicked:{
                    if(drawer.position===0){
                        drawer.open()
                    }else{
                        drawer.close()
                    }
                }
            }
            Button{
                id:newChatButton1
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
            }
            Item{
                Layout.fillWidth: true
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
            Button{
                id:newChatButton2
                Layout.topMargin: 10
                flat:true
                Layout.fillWidth: true
                contentItem: Item{
                    anchors.fill:parent
                    Row{
                        anchors.left: parent.left
                        leftPadding: 30
                        spacing: 20
                        Image{
                            source: "qrc:/img/icon/edit-05.svg"
                            width:18
                            height:18
                        }
                        Text{
                            text:"新聊天"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            // verticalAlignment: Text.AlignVCenter
                            // horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }

            Button{
                id:personalizationButton
                Layout.topMargin: 10
                flat:true
                Layout.fillWidth: true
                contentItem: Item{
                    anchors.fill:parent
                    Row{
                        anchors.left: parent.left
                        leftPadding: 30
                        spacing: 20
                        Image{
                            source: "qrc:/img/icon/cehua.svg"
                            width:18
                            height:18
                        }
                        Text{
                            text:"个性化定制"
                            font.family: "Microsoft YaHei"
                            font.pixelSize: 14
                            // verticalAlignment: Text.AlignVCenter
                            // horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
        }
    }
    Label {
        id: content
        text: "Aa"
        font.pixelSize: 96
        anchors.fill: parent
        verticalAlignment: Label.AlignVCenter
        horizontalAlignment: Label.AlignHCenter

        // transform: Translate {
        //     x: drawer.position * content.width * 0.33
        // }
    }
}
