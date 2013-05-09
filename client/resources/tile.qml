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
        source: city.thumbnailUrl
        anchors.horizontalCenter: parent.horizontalCenter
        z: -11
        transformOrigin: Item.Bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        fillMode: Image.Tile

        MouseArea {
            id: mouse_area1
            x: 0
            y: -200
            width: thumbnail.width * 0.6
            height: thumbnail.width * 0.6
            clip: true

            transformOrigin: Item.Center
            hoverEnabled: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            onEntered: {
                namePlate.visible = true;
            }

            onExited: {
                namePlate.visible = false;
            }

            onClicked:
            {
                view.setSelectedGuid(city.guid);
            }
        }

        Rectangle {
            id: uiHolder
            x: -101
            y: -143
            width: 201
            height: 286
            color: "#00000000"
            visible: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                id: loadState
                x: 1
                y: 65
                width: 200
                height: 29
                color: "#80ffffff"
                radius: 10
                visible: city.isLoading
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                border.color: "#3041c2"

                Text {
                    id: percent
                    x: 157
                    y: 0
                    width: 33
                    height: 29
                    text: Math.floor(city.transferPercent * 100) + "%";
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                }

                Rectangle {
                    x: 10
                    y: 10
                    width: 137
                    height: 8
                    color: "#ffffff"
                    radius: 2
                    border.width: 1
                    border.color: "#000000";

                    visible: city.isLoading

                    Rectangle {
                        id: progressIndicator
                        x: 1
                        y: 1
                        width: (parent.width * city.transferPercent) - 1
                        height: parent.height - 1
                        gradient: Gradient {
                            GradientStop {
                                position: 0.100
                                color: "#ffffff"
                            }

                            GradientStop {
                                position: 0.530
                                color: "#0ef70a"
                            }

                            GradientStop {
                                position: 0.850
                                color: "#1b2f02"
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: namePlate
                x: 0
                y: 0
                width: 200
                height: 60
                color: "#80ffffff"
                radius: 16
                visible: false
                anchors.horizontalCenterOffset: 0
                z: 22
                opacity: 1
                border.width: 2
                border.color: "#3041c2"

                Text {
                    id: name
                    x: 12
                    y: 9
                    width: 172
                    height: 18
                    text: city.name
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 12
                }

                Text {
                    id: owner
                    x: 44
                    y: 27
                    width: 112
                    height: 14
                    color: "#205a07"
                    text: city.owner
                    font.pointSize: 10
                    font.bold: true
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    horizontalAlignment: Text.AlignHCenter
                }

                AnimatedImage {
                    id: swirly
                    x: 163
                    y: 27
                    width: 28
                    height: 28
                    visible: city.isLoading
                    source: "ajax-loader.gif"
                }

                Rectangle {
                    id: button

                    property alias mouseAreaScale: mouseArea.scale

                    signal clicked
                    x: 44
                    y: 67

                    width: 112; height: 40
                    radius: 12
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#757070"
                        }

                        GradientStop {
                            position: 0.900
                            color: "#222233"
                        }
                    }

                    Behavior on scale { NumberAnimation { duration: 50 } }

                    MouseArea {
                        id: mouseArea

                        anchors.fill: parent
                        onClicked: { button.clicked() }
                        onEntered: { button.scale = 0.9 }
                        onExited: { button.scale = 1.0 }
                    }

                    Text {
                        id: claimText
                        x: 21
                        y: 13
                        width: 71
                        height: 14
                        color: "#ffffff"
                        text: "Claim City"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 12
                    }
                }

                Rectangle {
                    x: 98
                    y: 60
                    width: 5
                    height: 103
                    radius: 0
                    z: -10
                    border.width: 0
                    border.color: "#000000"
                    anchors.horizontalCenter: parent.horizontalCenter
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#4133e0"
                        }

                        GradientStop {
                            position: 1
                            color: "#00000000"
                        }
                    }
                    visible: true
                }
            }

            Image {
                id: image1
                x: 110
                y: 113
                width: 55
                height: 49
                source: "flag.png"
            }
        }



    }
}
