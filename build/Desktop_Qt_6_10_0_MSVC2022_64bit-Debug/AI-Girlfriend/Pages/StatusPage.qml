import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    
    background: Rectangle { color: "#f0f0f4" }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 30
        
        Label {
            text: qsTr("服务器状态")
            font.family: "Microsoft YaHei"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Grid {
            columns: 2
            spacing: 20
            
            // ASR Status
            Rectangle {
                width: 200
                height: 120
                color: "#FFFFFF"
                radius: 10
                
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10
                    
                    Image {
                        source: "qrc:/img/icon/microphone-01.svg"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                    }
                    Label {
                        text: "ASR 服务"
                        font.family: "Microsoft YaHei"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Label {
                        text: "已连接"
                        color: "green"
                        font.family: "Microsoft YaHei"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }

            // LLM Status
            Rectangle {
                width: 200
                height: 120
                color: "#FFFFFF"
                radius: 10
                
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10
                    
                    Image {
                        source: "qrc:/img/icon/shendusikao.svg"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                    }
                    Label {
                        text: "LLM 服务"
                        font.family: "Microsoft YaHei"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Label {
                        text: "运行中"
                        color: "green"
                        font.family: "Microsoft YaHei"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
            
            // TTS Status
            Rectangle {
                width: 200
                height: 120
                color: "#FFFFFF"
                radius: 10
                
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10
                    
                    Image {
                        source: "qrc:/img/icon/music.svg"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                    }
                    Label {
                        text: "TTS 服务"
                        font.family: "Microsoft YaHei"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Label {
                        text: "已连接"
                        color: "green"
                        font.family: "Microsoft YaHei"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }
    }
}
