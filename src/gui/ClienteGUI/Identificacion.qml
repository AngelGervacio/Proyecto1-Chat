import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
anchors.fill: parent

    Rectangle{
        width: 250
        height: 150
        anchors.centerIn: parent
        color: "whitesmoke"
        border.width: 2
        border.color: "gainsboro"
        radius: 10

        ColumnLayout{
            anchors.centerIn: parent
            spacing: 12

            Label{
                text: "Nombre de Usuario"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignHCenter
            }

            TextField{
                id: nombreUsuario
                maximumLength: 8
                font.pixelSize: 12
                Layout.preferredWidth: 120
                focus: true
                horizontalAlignment: TextInput.AlignHCenter
            }

            Button{
                Layout.alignment: Qt.AlignHCenter
                text: "Conectarse"

                onClicked:{
                    if(nombreUsuario.text.trim() === ""){
                        nombreUsuario.clear
                        return
                    }

                    controlador.identificarse(nombreUsuario.text)
                }
            }
        }
    }

    Dialog{
        id: dialogoYaExiste

        width: 250
        height: 100
        anchors.centerIn: parent
        title: "Ya existe"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoYaExiste.mensaje
        }
    }

    Connections{
        target: controlador

        function onIdentificacionYaExiste(nombre){
            dialogoYaExiste.mensaje = "El usuario " + nombre + " ya existe."
            dialogoYaExiste.open()
        }
    }
}
