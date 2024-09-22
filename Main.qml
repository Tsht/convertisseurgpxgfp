import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtCore

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Convertisseur GPX vers GFP (Garmin)")
    ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Button {
                text: qsTr("Sélectionner le fichier GPX source")
                onClicked: {
                    fileDialog.open()
                }
            }

            TextField {
                id: inputPath
                placeholderText: qsTr("Chemin du fichier source")
                readOnly: true
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Choisir l'emplacement de sauvegarde pour le fichier GFP")
                onClicked: {
                    saveFileDialog.open()
                }
            }

            TextField {
                id: outputPath
                placeholderText: qsTr("Chemin du fichier de sortie")
                readOnly: true
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Convertir")
                onClicked: {
                    if (inputPath.text !== "" && outputPath.text !== "") {
                        // Appeler la fonction C++ pour la conversion
                        var result = converter.convert(inputPath.text, outputPath.text);
                        if (result) {
                            messageDialog.text = qsTr("Le fichier a été converti avec succès.");
                        } else {
                            messageDialog.text = qsTr("Erreur lors de la conversion.");
                        }
                        messageDialog.open();
                    } else {
                        messageDialog.text = qsTr("Veuillez sélectionner les fichiers d'entrée et de sortie.");
                        messageDialog.open();
                    }
                }
            }

            MessageDialog {
                id: messageDialog
                title: qsTr("Information")
                buttons: MessageDialog.Ok
                onAccepted: messageDialog.close()
                Text {
                    text: messageDialog.text
                    wrapMode: Text.Wrap
                }
            }
        }

        FileDialog {
            id: fileDialog
            title: qsTr("Ouvrir un fichier GPX")
            currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            fileMode: FileDialog.OpenFile
            nameFilters: ["Fichiers GPX (*.gpx)"]
            onAccepted: {
                    inputPath.text = selectedFile
                }
            onRejected: {
                // L'utilisateur a annulé, aucune action nécessaire
            }
        }

        FileDialog {
            id: saveFileDialog
            title: qsTr("Enregistrer le fichier GFP (Garmin)")
            defaultSuffix: "gfp"
            currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            fileMode: FileDialog.SaveFile
            nameFilters: ["Fichier GFP (*.gfp)"]
            onAccepted: {
                  outputPath.text = selectedFile
                }

            onRejected: {
                // L'utilisateur a annulé, aucune action nécessaire
            }
        }
}

