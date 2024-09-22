#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = nullptr);
public slots:
    bool convert(const QString &inputPath, const QString &outputPath);
private:
    QString convertCoordinate(double latitude, double longitude);
};

#endif // CONVERTER_H
