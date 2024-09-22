#include "converter.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QGeoCoordinate>
#include <QUrl>

Converter::Converter(QObject *parent) : QObject(parent) {}

bool Converter::convert(const QString &inputPath, const QString &outputPath)
{
    QUrl inputUrl(inputPath);
    QUrl outputUrl(outputPath);
    if(!inputUrl.isLocalFile() || !outputUrl.isLocalFile())
    {
        return false;
    }

    QFile inputFile(inputUrl.toLocalFile());
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QFile outputFile(outputUrl.toLocalFile());
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QXmlStreamReader xmlReader(&inputFile);
    QTextStream outputStream(&outputFile);

    QString outputString = "FPN/RI";

    // Liste pour stocker les coordonnées
    QList<QPair<double, double>> coordinates;

    // Parcours du XML
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            QString elementName = xmlReader.name().toString();

            if (elementName == "rtept") {
                // Extraction des attributs lat et lon
                QXmlStreamAttributes attributes = xmlReader.attributes();
                double lat = attributes.value("lat").toDouble();
                double lon = attributes.value("lon").toDouble();

                coordinates.append(qMakePair(lat, lon));
            }
        }
    }

    if (xmlReader.hasError()) {
        inputFile.close();
        outputFile.close();
        return false;
    }

    // Conversion des coordonnées et construction de la chaîne de sortie
    for (const auto &coord : coordinates) {
        QString coordString = convertCoordinate(coord.first, coord.second);
        outputString += ":F:" + coordString;
    }

    // Écriture dans le fichier de sortie
    outputStream << outputString;

    inputFile.close();
    outputFile.close();

    return true;
}

QString Converter::convertCoordinate(double latitude, double longitude)
{
    QGeoCoordinate coordinate(latitude, longitude);

    // Latitude
    QString latDirection = coordinate.latitude() >= 0 ? "N" : "S";
    double lat = qAbs(coordinate.latitude());
    int latDegrees = static_cast<int>(lat);
    double latMinutesTotal = (lat - latDegrees) * 60;
    int latMinutes = static_cast<int>(latMinutesTotal);
    double latSeconds = (latMinutesTotal - latMinutes) * 60;
    int latTenths = qRound(latSeconds / 6.0);

    // Longitude
    QString lonDirection = coordinate.longitude() >= 0 ? "E" : "W";
    double lon = qAbs(coordinate.longitude());
    int lonDegrees = static_cast<int>(lon);
    double lonMinutesTotal = (lon - lonDegrees) * 60;
    int lonMinutes = static_cast<int>(lonMinutesTotal);
    double lonSeconds = (lonMinutesTotal - lonMinutes) * 60;
    int lonTenths = qRound(lonSeconds / 6.0);

    // Formatage des chaînes
    QString latString = QString("%1%2%3%4")
                            .arg(latDirection)
                            .arg(latDegrees, 2, 10, QChar('0'))
                            .arg(latMinutes, 2, 10, QChar('0'))
                            .arg(latTenths);

    QString lonString = QString("%1%2%3%4")
                            .arg(lonDirection)
                            .arg(lonDegrees, 3, 10, QChar('0'))
                            .arg(lonMinutes, 2, 10, QChar('0'))
                            .arg(lonTenths);

    // Combinaison des deux
    QString coordString = latString + lonString;

    return coordString;
}
