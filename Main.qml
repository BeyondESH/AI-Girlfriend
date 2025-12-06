import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components"
import "Pages"

ApplicationWindow{
    id:window
    width: 1024
    height: 800
    visible: true
    title: qsTr("你的女孩")
    minimumWidth:1024
    minimumHeight:800
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
                        if (app) app.createNewConversation()
                        stackView.replace("Pages/ChatPage.qml")
                    }
                }
            }
            ComboBox{
                id: modelComboBox
                Layout.preferredWidth: 150
                Layout.preferredHeight: 30
                editable: true
                flat:true
                model: ListModel {
                    id: modelListModel
                    ListElement { text: "Qwen3:8b" }
                    ListElement { text: "Llama3.2" }
                    ListElement { text: "Qwen3-vl:8b" }
                }
                
                Component.onCompleted: {
                    // 从配置加载当前选中的模型
                    var currentModel = configMgr.llmModelName
                    var found = false
                    for (var i = 0; i < modelListModel.count; i++) {
                        if (modelListModel.get(i).text === currentModel) {
                            currentIndex = i
                            found = true
                            break
                        }
                    }
                    if (!found && currentModel !== "") {
                        modelListModel.append({text: currentModel})
                        currentIndex = modelListModel.count - 1
                    }
                }
                
                onAccepted: {
                    if (find(editText) === -1)
                        modelListModel.append({text: editText})
                }
                
                onCurrentTextChanged: {
                    if (currentText !== "") {
                        configMgr.llmModelName = currentText
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: qsTr("选择大模型")
            }

            Label{
                id:stateLabel
                Layout.alignment: Qt.AlignCenter
                text:"雅萱正在输入..."
                font.family: "Microsoft YaHei"
                font.pointSize: 10
                visible: false
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
                onClicked: {
                    if (app) app.createNewConversation()
                    stackView.replace("Pages/ChatPage.qml")
                }
            }

            SideButton{
                id:settingSideButton
                text:qsTr("个性化定制")
                image:"qrc:/img/icon/user-02.svg"
                onClicked:{
                    stackView.replace("Pages/SettingPage.qml")
                }
            }

            SideButton{
                id:stateSideButton
                text:qsTr("服务器状态")
                image:"qrc:/img/icon/cehua.svg"
                onClicked: {
                    stackView.replace("Pages/StatePage.qml")
                }
            }

            SideButton{
                id:voiceChatSideButton
                text:qsTr("语音对话")
                image:"qrc:/img/icon/microphone-01.svg"
                onClicked: {
                    stackView.replace("Pages/VoiceChatPage.qml")
                }
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
                    model: app ? app.conversationList : []
                    spacing: 5
                    clip: true

                    delegate: Button {
                        id: delegateButton
                        width: ListView.view.width
                        height: 40
                        flat: true
                        
                        property string convId: modelData.id || ""
                        property string convTitle: modelData.title || "新对话"
                        
                        contentItem: RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 15
                            anchors.rightMargin: 10
                            spacing: 8
                            
                            Image {
                                source: "qrc:/img/icon/message-chat-square.svg"
                                Layout.preferredWidth: 16
                                Layout.preferredHeight: 16
                                opacity: 0.6
                            }
                            
                            Text {
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                                clip: true
                                font.family: "Microsoft YaHei"
                                font.pixelSize: 13
                                text: delegateButton.convTitle
                                color: (app && delegateButton.convId === app.currentConversationId) ? "#0078d4" : "#4b4b4b"
                            }
                            
                            // 删除按钮
                            Button {
                                id: deleteBtn
                                Layout.preferredWidth: 20
                                Layout.preferredHeight: 20
                                flat: true
                                opacity: delegateButton.hovered ? 1 : 0
                                
                                Behavior on opacity {
                                    NumberAnimation { duration: 150 }
                                }
                                
                                contentItem: Text {
                                    text: "×"
                                    font.pixelSize: 14
                                    color: "#666"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                
                                background: Rectangle {
                                    radius: 3
                                    color: deleteBtn.hovered ? "#e0e0e0" : "transparent"
                                }
                                
                                onClicked: {
                                    deleteConfirmDialog.convIdToDelete = delegateButton.convId
                                    deleteConfirmDialog.open()
                                }
                            }
                        }
                        
                        background: Rectangle {
                            radius: 6
                            color: {
                                if (app && delegateButton.convId === app.currentConversationId)
                                    return "#e6f3ff"
                                else if (delegateButton.hovered)
                                    return "#f5f5f5"
                                else
                                    return "transparent"
                            }
                            
                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                        }
                        
                        onClicked: {
                            // 点击按钮主体时加载对话
                            if (app) {
                                app.loadConversation(delegateButton.convId)
                                stackView.replace("Pages/ChatPage.qml")
                            }
                        }
                    }
                    
                    // 空状态提示
                    Label {
                        anchors.centerIn: parent
                        text: "暂无历史对话"
                        color: "#999"
                        font.family: "Microsoft YaHei"
                        font.pixelSize: 12
                        visible: historyListView.count === 0
                    }
                }
            }
        }
    }
    
    // 删除确认对话框
    Dialog {
        id: deleteConfirmDialog
        title: "确认删除"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel
        
        property string convIdToDelete: ""
        
        Label {
            text: "确定要删除这个对话吗？此操作不可恢复。"
            font.family: "Microsoft YaHei"
        }
        
        onAccepted: {
            if (convIdToDelete !== "" && app) {
                app.deleteConversation(convIdToDelete)
                convIdToDelete = ""
            }
        }
    }
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: ChatPage {
            onSignal_sendMessage: (text) => {
                stateLabel.visible=true
            }
        }
    }
}
