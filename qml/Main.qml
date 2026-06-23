import License 1.0
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 500
    height: 500

    LicenseGenerator {
        id: generator
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: device

            placeholderText: "device hwid"
        }

        Button {
            text: "Generate"
            onClicked: {
                let key = generator.generate(device.text);
                result.text = key;
            }
        }

        TextArea {
            id: result

            width: 400
            height: 150
            wrapMode: Text.WrapAnywhere
        }
    }
}
