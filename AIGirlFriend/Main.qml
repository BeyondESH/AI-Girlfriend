import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: qsTr("你的女孩")

    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 20

            Button {
                id: connectButton
                text: "连接服务器"
                width: 150
                height: 40
                onClicked: {
                    websocketMgr.sendConfig(audioMgr.sampleRate);
                }
            }

            Button {
                id: playButton
                text: "开始录制"
                width: 150
                height: 40
                onClicked: {
                    audioMgr.start();
                }
            }

            Button {
                id: stopButton
                text: "结束录制"
                width: 150
                height: 40
                onClicked: {
                    audioMgr.stop();
                }
            }
        }
    }
}
