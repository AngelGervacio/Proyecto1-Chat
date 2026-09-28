import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
    id: chat
    anchors.fill: parent
    property string usuario: ""
    property string chatActual: ""
    property string tipoChat: ""
    property var historiales:({})

    signal desconexion()

    function seUnio(){
        chat.agregaMensajeHistorial(chat.chatActual, "Te has unido.")
        mensajes.insert(0, {"mensaje":"Te has unido."})
    }

    function creaHistorial(nombre){
        if(historiales[nombre] === undefined)
            historiales[nombre] = []
    }

    function agregaMensajeHistorial(chat, mensaje){
        creaHistorial(chat)
        historiales[chat].push(mensaje)
    }

    onChatActualChanged:{
        mensajes.clear()

        let historial = historiales[chatActual]

        if(historial !== undefined)
            for(let mensaje of historial)
                mensajes.insert(0, {"mensaje": mensaje})

        if(chat.tipoChat === "SALA")
            controlador.obtenerUsuariosSala(chat.chatActual)
    }

    function buscar(nombre){
        for(let i = 0; i < listaUsuarios.count; i++){
            if(listaUsuarios.get(i).nombre === nombre)
                return i
        }
        return -1
    }

    function buscarChat(nombre, tipo){
        for(let i = 0; i < chats.count; i++){
            if(chats.get(i).nombre == nombre && chats.get(i).tipo == tipo)
                return i
        }
        return -1
    }

    function seleccionaUsuario(nombre){
        if(chat.usuario === nombre)
            return

        let i = buscarChat(nombre, "PRIVADO")

        if(i === -1)
            chats.append({"nombre": nombre, "tipo": "PRIVADO"})

        chat.tipoChat = "PRIVADO"
        chat.chatActual = nombre
        chat.actualizaListaPrivada()
    }

    function actualizaListaPrivada(){
        listaPrivada.clear()

        let i = buscar(chat.usuario)

        if(i !== -1)
            listaPrivada.append({"nombre": listaUsuarios.get(i).nombre,
                                "estatus": listaUsuarios.get(i).estatus})

        i = chat.buscar(chat.chatActual)

        if(i !== -1)
            listaPrivada.append({"nombre": listaUsuarios.get(i).nombre,
                                "estatus": listaUsuarios.get(i).estatus})
    }

    Component.onCompleted:{
        controlador.obtenerUsuarios()

        creaHistorial("Publico")
        chats.append({"nombre": "Publico", "tipo": "PUBLICO"})
        chat.chatActual = "Publico"
        chat.tipoChat = "PUBLICO"
    }

    RowLayout{
        anchors.fill: parent

        Rectangle{
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "aliceblue"

            ColumnLayout{
                anchors.fill: parent
                anchors.margins: 14

                ListModel{
                    id:chats
                }

                ListModel{
                    id: invitaciones
                }

                ListView{
                    id: listaChatsView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    spacing: 10

                    model: chats

                    delegate: Rectangle{
                        id: chatRectangulo
                        width: listaChatsView.width
                        height: 35
                        radius: 10
                        color: mouseArea.containsMouse ? "lavender" : "white"

                        Text{
                            anchors.fill: parent
                            anchors.margins: 12
                            text: nombre
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }

                        MouseArea{
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked:{
                                chat.tipoChat = tipo
                                chat.chatActual = nombre

                                if(chat.tipoChat === "PRIVADO")
                                    actualizaListaPrivada()
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {}
                }

                RowLayout{
                    anchors.margins: 12
                    Button{
                        text: "Invitaciones"
                        Layout.fillWidth: true
                        Layout.alignment: Text.AlignVCenter

                        onClicked:{
                            dialogoInvitaciones.open()
                        }
                    }
                }

                RowLayout{
                    anchors.margins: 12
                    Button{
                        text: "Crear Sala"
                        Layout.fillWidth: true
                        Layout.alignment: Text.AlignVCenter

                        onClicked:{
                            dialogoCreaSala.open()
                        }
                    }
                }

                RowLayout{
                    anchors.margins: 12
                    visible: chat.tipoChat === "SALA"
                    Button{
                        text: "Salir de la Sala"
                        Layout.fillWidth: true
                        Layout.alignment: Text.AlignVCenter

                        onClicked:{
                            dialogoSalirSala.mensaje = "¿Seguro que quieres salir de " + chat.chatActual + "?"
                            dialogoSalirSala.open()
                        }
                    }
                }

                ColumnLayout{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text{
                        text: chat.usuario
                    }

                    RowLayout{

                        Rectangle{
                            Layout.preferredWidth: 15
                            Layout.preferredHeight: 15
                            radius: width / 2

                            border.width: 2

                            color:{
                                switch(selectorEstatus.currentIndex){
                                case 0: return "green"
                                case 1: return "yellow"
                                case 2: return "red"
                                default: return "white"
                                }
                            }
                        }

                        ComboBox{
                            id: selectorEstatus
                            model: ["ACTIVE", "AWAY", "BUSY"]

                            onCurrentIndexChanged:{
                                controlador.cambiarEstatus(currentIndex)

                                let i = chat.buscar(chat.usuario)
                                if(i !== -1)
                                    listaUsuarios.setProperty(i, "estatus", currentIndex)

                                if(chat.tipoChat === "PRIVADO")
                                    chat.actualizaListaPrivada()

                                if(chat.tipoChat === "SALA")
                                    for(let j = 0; j < listaSala.count; j++){
                                        if(listaSala.get(j).nombre === chat.usuario){
                                            listaSala.setProperty(j, "estatus", currentIndex)
                                        }
                                    }
                            }
                        }
                    }

                    Button{
                        text: "Desconectarse"

                        onClicked:{
                            controlador.desconectar()
                            Qt.quit()
                        }
                    }
                }
            }
        }

        ColumnLayout{
            Layout.fillHeight: true
            Layout.preferredWidth: 560
            Layout.margins: 14

            ListModel{
                id: mensajes
            }

            ListView{
                id: chatMensajes
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                verticalLayoutDirection: ListView.VerticalBottomToTop
                model: mensajes

                delegate: Text{
                    text: mensaje
                }

                ScrollBar.vertical: ScrollBar {}
            }

            RowLayout{
                Layout.fillWidth: true

                TextField{
                    id: campoTexto
                    Layout.fillWidth: true
                }

                Button{
                    text: "Enviar"

                    onClicked:{
                        if(campoTexto.text.trim() === ""){
                            campoTexto.clear()
                            return;
                        }

                        if(chat.tipoChat === "PRIVADO"){
                            controlador.enviarTexto(chat.chatActual, campoTexto.text)
                        }else if(chat.tipoChat === "PUBLICO"){
                            controlador.enviarTextoPublico(campoTexto.text)
                        }else if(chat.tipoChat === "SALA"){
                            controlador.enviarTextoSala(chat.chatActual, campoTexto.text)
                        }

                        chat.agregaMensajeHistorial(chat.chatActual, chat.usuario + ": " + campoTexto.text)
                        mensajes.insert(0, {"mensaje": chat.usuario + ": " + campoTexto.text})

                        campoTexto.clear()
                    }
                }
            }
        }

        Rectangle{
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "aliceblue"

            ColumnLayout{
                anchors.fill: parent
                anchors.margins: 14

                ListModel{
                    id: listaUsuarios
                }

                ListModel{
                    id: listaSala
                }

                ListModel{
                    id: listaPrivada
                }

                ListView{
                    id: listaUsuariosView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    spacing: 5
                    model: chat.tipoChat === "PRIVADO" ? listaPrivada :
                            chat.tipoChat === "SALA" ? listaSala : listaUsuarios


                    delegate: Rectangle{
                        width: listaUsuariosView.width
                        height: 30
                        radius: 10
                        color: mouseAreaLista.containsMouse ? "lavender" : "transparent"

                        MouseArea{
                            id: mouseAreaLista
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked:{
                                seleccionaUsuario(nombre)
                            }
                        }

                        RowLayout{
                            anchors.fill: parent

                            Rectangle{
                                Layout.preferredWidth: 15
                                Layout.preferredHeight: 15
                                radius: width / 2

                                border.width: 2

                                color:{
                                    switch(estatus){
                                    case 0: return "green"
                                    case 1: return "yellow"
                                    case 2: return "red"
                                    default: return "white"
                                    }
                                }
                            }

                            Text{
                                Layout.fillWidth: true
                                text: nombre
                                horizontalAlignment: Text.AlignLeft
                            }

                            Button{
                                visible: chat.tipoChat === "SALA" && !enSala
                                text: "Invitar"

                                onClicked:{
                                    controlador.invitar([nombre], chat.chatActual)
                                }
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {}
                }
            }
        }
    }

    Dialog{
        id: dialogoNoEncontrado

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "Usuario no conectado"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoNoEncontrado.mensaje
        }
    }

    Dialog{
        id: dialogoCreaSala

        width: 300
        height: 100
        anchors.centerIn: parent
        title: "Crear Sala"
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        contentItem: TextField{
            id: campoNombreSala
            maximumLength: 16
        }

        onAccepted:{
            if(campoNombreSala.text.trim() === "")
                return

            controlador.crearSala(campoNombreSala.text)
            campoNombreSala.clear()
        }

        onRejected: campoNombreSala.clear()
    }

    Dialog{
        id: dialogoSalaYaExiste

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "Sala Ya Existe"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoSalaYaExiste.mensaje
        }
    }

    Dialog{
        id:dialogoInvitaciones

        width: 500
        height: 300
        anchors.centerIn: parent
        title: "Invitaciones"
        modal: true
        standardButtons: Dialog.Close

        ListView{
            anchors.fill: parent
            model: invitaciones

            delegate: RowLayout{
                width: ListView.view.width
                spacing: 10

                Text{
                    Layout.fillWidth: true
                    text: usuario + " te invito a " + sala
                }

                Button{
                    text: "Unirte"

                    onClicked:{
                        controlador.aceptarInvitacion(sala)
                        invitaciones.remove(index)
                    }
                }
            }
        }
    }

    Dialog{
        id: dialogoSalaNoExiste

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "Sala No Existe"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoSalaNoExiste.mensaje
        }
    }

    Dialog{
        id: dialogoInvitacionAceptada

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "Invitacion Aceptada"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoInvitacionAceptada.mensaje
        }
    }

    Dialog{
        id: dialogoNoInvitado

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "No Invitado"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoNoInvitado.mensaje
        }
    }

    Dialog{
        id: dialogoNoUnido

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "No Unido"
        modal: true
        standardButtons: Dialog.Ok

        property string mensaje: ""

        Text{
            text: dialogoNoUnido.mensaje
        }
    }

    Dialog{
        id: dialogoSalirSala

        width: 500
        height: 100
        anchors.centerIn: parent
        title: "Salir de la Sala"
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        property string mensaje: ""

        Text{
            text: dialogoSalirSala.mensaje
        }

        onAccepted:{
            let sala = chat.chatActual
            controlador.salirSala(sala)

            let i = buscarChat(sala, "SALA")

            if(i !== -1)
                chats.remove(i)

            chat.tipoChat = "PUBLICO"
            chat.chatActual = "Publico"
        }
    }

    Connections{
        target: controlador

        function onNuevoUsuario(usuario){
            if(chat.chatActual === "Publico")
                mensajes.insert(0, {"mensaje": usuario + " se ha unido."})

            listaUsuarios.append({"nombre": usuario, "estatus": 0})

            if(chat.tipoChat === "SALA")
                listaSala.append({"nombre": usuario, "estatus": 0, "enSala": false})
        }

        function onListaUsuarios(lista){
            for(let usuario in lista){
                listaUsuarios.append({
                    "nombre": usuario,
                    "estatus": lista[usuario]
                })
            }
        }

        function onNuevoEstatus(usuario, estatus){
            let i = chat.buscar(usuario)

            if(i !== -1)
                listaUsuarios.setProperty(i, "estatus", estatus)

            if(chat.tipoChat === "PRIVADO")
                chat.actualizaListaPrivada()
            else if(chat.tipoChat === "SALA")
                for(let j = 0; j < listaSala.count; j++){
                    if(listaSala.get(j).nombre === usuario){
                        listaSala.setProperty(j, "estatus", estatus)
                        break
                    }
                }
        }

        function onMensajePrivado(usuario, texto){
            let i = chat.buscarChat(usuario, "PRIVADO")

            if(i === -1)
                chats.append({"nombre": usuario, "tipo": "PRIVADO"})

            chat.agregaMensajeHistorial(usuario, usuario + ": " + texto)
            if(chat.chatActual === usuario)
                mensajes.insert(0, {"mensaje": usuario + ": " + texto})
        }

        function onUsuarioNoEncontrado(usuario){
            dialogoNoEncontrado.mensaje = "El usuario " + usuario + " no se encuentra."
            dialogoNoEncontrado.open()
        }

        function onMensajePublico(usuario, texto){
            chat.agregaMensajeHistorial("Publico", usuario + ": " + texto)
            if(chat.chatActual === "Publico")
                mensajes.insert(0, {"mensaje": usuario + ": " + texto})
        }

        function onSalaCreada(sala){
            chats.append({"nombre": sala, "tipo": "SALA"})

            delete chat.historiales[sala]

            chat.tipoChat = "SALA"
            chat.chatActual = sala
            chat.seUnio()
        }

        function onSalaYaExiste(sala){
            dialogoSalaYaExiste.mensaje = "La sala " + sala + " ya existe."
            dialogoSalaYaExiste.open()
        }

        function onListaSala(lista){
            listaSala.clear()

            for(let usuario in lista){
                listaSala.append({"nombre": usuario, "estatus": lista[usuario], "enSala": true})
            }

            for(let i = 0; i < listaUsuarios.count; i++){
                let usuario = listaUsuarios.get(i).nombre

                if(lista[usuario] === undefined)
                    listaSala.append({"nombre": usuario, "estatus": listaUsuarios.get(i).estatus, "enSala": false})
            }
        }

        function onInvitacionRecibida(usuario, sala){
            invitaciones.append({"usuario": usuario, "sala": sala})
        }

        function onSalaNoExiste(sala){
            dialogoSalaNoExiste.mensaje = "La sala " + sala + " no existe."
            dialogoSalaNoExiste.open()
        }

        function onInvitacionAceptada(sala){
            if(chat.buscarChat(sala, "SALA") === -1)
                chats.append({"nombre": sala, "tipo": "SALA"})

            delete chat.historiales[sala]

            chat.tipoChat = "SALA"
            chat.chatActual = sala

            dialogoInvitacionAceptada.mensaje = "Te has unido a " + sala + "."
            dialogoInvitacionAceptada.open()

            chat.agregaMensajeHistorial(sala, "Te has unido a la sala.")
            if(chat.chatActual === sala)
                mensajes.insert(0, {"mensaje": "Te has unido a la sala."})
        }

        function onNuevoUsuarioSala(sala, usuario){
            chat.agregaMensajeHistorial(sala, usuario + " se ha unido a la sala.")
            if(chat.chatActual === sala)
                mensajes.insert(0, {"mensaje": usuario + " se ha unido a la sala."})


            for(let i = 0; i < listaSala.count; i++){
                if(listaSala.get(i).nombre === usuario){
                    listaSala.setProperty(i, "enSala", true)
                    break;
                }
            }
        }

        function onNoInvitado(sala){
            dialogoNoInvitado.mensaje = "No has sido invitado a " + sala
            dialogoNoInvitado.open()
        }

        function onNoUnido(sala){
            dialogoNoUnido.mensaje = "No te has unido a " + sala
            dialogoNoUnido.open()
        }

        function onMensajeSala(sala, usuario, texto){
            chat.agregaMensajeHistorial(sala, usuario + ": " + texto)
            if(chat.chatActual === sala)
                mensajes.insert(0, {"mensaje": usuario + ": " + texto})
        }

        function onSalioDeSala(sala, usuario){
            chat.agregaMensajeHistorial(sala, usuario + " salio de la sala.")
            if(chat.chatActual === sala){
                mensajes.insert(0, {"mensaje": usuario + " salio de la sala."})
                for(let j = 0; j < listaSala.count; j++){
                    if(listaSala.get(j).nombre === usuario){
                        listaSala.remove(j)
                        break
                    }
                }
            }
        }

        function onDesconectado(usuario){
            let i = chat.buscar(usuario)
            chat.agregaMensajeHistorial("Publico", usuario + " se ha desconectado.")
            if(chat.chatActual === "Publico")
                mensajes.insert(0, {"mensaje": usuario + " se ha desconectado. "})

            if(i !== -1)
                listaUsuarios.remove(i)

            if(chat.tipoChat === "PRIVADO")
                chat.actualizaListaPrivada()

            if(chat.tipoChat === "SALA")
                for(let j = 0; j < listaSala.count; j++){
                    if(listaSala.get(j).nombre === usuario){
                        listaSala.remove(j)
                        break
                    }
                }
        }
    }
}
