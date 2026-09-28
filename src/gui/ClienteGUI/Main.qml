import QtQuick
//import QtQuick.Controls
//import QtQuick.Layouts

Window {
    id: ventana
    width: 960
    height: 540
    visible: true
    title: qsTr("ChatCliente")

    property bool identificado: false
    property string usuario: ""

    Loader{
        anchors.fill: parent
        source: ventana.identificado ? "Chat.qml" : "Identificacion.qml"

        onLoaded:{
            if(ventana.identificado){
                item.usuario = ventana.usuario
                item.seUnio()
            }
        }
    }

    Connections{
        target: controlador

        function onIdentificacionExitosa(nombre){
            ventana.usuario = nombre
            ventana.identificado = true
        }
    }
}
