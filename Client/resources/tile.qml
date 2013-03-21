// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: tile
    width: city.screenWidth()
    height: 400
    color: "#00000000"

    Image {
        id: thumbnail
        x: 0
        y: 100
        anchors.horizontalCenter: parent.horizontalCenter
        z: -11
        transformOrigin: Item.Bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        fillMode: Image.Tile
        source: city.thumbnailUrl


    }

    Rectangle {
        id: rectangle1
        x: -100
        y: 234
        width: 200
        height: 60
        color: "#80ffffff"
        radius: 16
        anchors.horizontalCenterOffset: 0
        z: 22
        anchors.horizontalCenter: thumbnail.horizontalCenter
        opacity: 1
        border.width: 2
        border.color: "#000000"

        Text {
            id: name
            x: 12
            y: 9
            width: 172
            height: 18
            text: city.name()
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: text1
            x: 44
            y: 27
            width: 112
            height: 14
            color: "#205a07"
            text: city.guid()
            font.pointSize: 10
            font.bold: true
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
        }

        AnimatedImage {
            id: image1
            x: 163
            y: 27
            width: 28
            height: 28
            source: "ajax-loader.gif"
        }
    }
}
