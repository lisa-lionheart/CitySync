// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: rectangle2
    width: 800
    height: 500
    color: "#00000000"

    Rectangle {
        id: rectangle1
        x: 313
        y: 328
        width: 467
        height: 159
        color: "#808e9acf"
        radius: 36
        anchors.right: parent.right
        anchors.rightMargin: 20
        transformOrigin: Item.Right
        border.width: 2
        border.color: "#000000"
    }
}
