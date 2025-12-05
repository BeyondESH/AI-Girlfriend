import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    id:root
    Layout.topMargin: 10
    Layout.fillWidth: true
    Layout.preferredHeight:item.implicitHeight+20
    property alias text: text.text
    property alias image:image.source
    signal clicked()
    Button{
        id:button
        flat:true
        anchors.fill:parent
        contentItem: Item{
            id:item
            implicitHeight: row.implicitHeight
            width:parent.width
            Row{
                id:row
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 10
                spacing: 20
                Image{
                    id:image
                    source: "qrc:/img/icon/edit-05.svg"
                    width:18
                    height:18
                }
                Text{
                    id:text
                    text:qsTr("新聊天")
                    font.family: "Microsoft YaHei"
                    font.pixelSize: 14
                    // verticalAlignment: Text.AlignVCenter
                    // horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        MouseArea{
            anchors.fill:parent
            cursorShape: Qt.PointingHandCursor
            onClicked:{
                root.clicked()
            }
        }
    }
}
