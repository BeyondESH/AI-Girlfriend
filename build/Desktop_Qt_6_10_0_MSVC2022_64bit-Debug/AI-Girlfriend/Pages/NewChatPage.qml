import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    id:newChatPage
    Row {
        spacing: 10

        Button {
            text: "Push"
            onClicked: stack.push(newChatPage)
        }
        Button {
            text: "Pop"
            enabled: stack.depth > 1
            onClicked: stack.pop()

        }
        Text {
            text: stack.depth
        }
    }
}
