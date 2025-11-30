import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ApplicationWindow{
    width: 1024
    height: 800
    visible: true
    title: qsTr("Hello World")

    header:Rectangle{
        height: headerLayout.implicitHeight
        width:parent.width
        // color:"lightgray"
        RowLayout{
            id:headerLayout
            anchors.fill:parent
            spacing:10
            Button{
                Layout.leftMargin: 25
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                icon.name: "Address-BookNew"
                // icon.height: 30
                // icon.width: 30
                // text:"弹"
                flat:true
            }
            Button{
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                text:"新"
                flat:true
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
}
