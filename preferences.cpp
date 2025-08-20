#include "preferences.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Preferences::Preferences()
{
    /// В конструкторе происходит только инициализация значений
    initValues();
}
/// Устанавливаются значения по умолчанию, которые потом поменяются при загрузке данных из .xml (кроме первого запуска)
void Preferences::initValues()
{
    tempVal = 20.0;
    humidityVal = 45.0;
    pressureVal = 760.0;
    tempUnit = "°C";
    pressureUnit = "Pa";
    targetTemp = 20.0;
    acAngle = 0.0;
    resolution = QSize(800,600);
    darkTheme = true;
    power = true;
    tempMin = -40;
    tempMax = 60;
    humidityMin = 0;
    humidityMax = 100;
    pressureMin = 500;
    pressureMax = 900;
}

void Preferences::setLimits()
{
    /// Минимальная температура в цельсиях
    tempMin = -40;
    tempMax = 60;

    /// Затем температура меняется в зависимости от единиц измерения
    if (tempUnit == "°F") {
        tempMin = (tempMin * 9 / 5) + 32;
        tempMax = (tempMax * 9 / 5) + 32;
    } else if (tempUnit == "°K") {
        tempMin += 273.15;
        tempMax += 273.15;
    }

    /// Минимальное давление в мм рт.ст.
    pressureMin = 500;
    pressureMax = 900;
    /// Затем давление меняется в зависимости от единиц измерения
    if (pressureUnit == "Pa") {
        pressureMin *= 133.322;
        pressureMax *= 133.322;
    }
}

void Preferences::setResolution()
{
    /// Переключение между двумя разрешениями окна
    if (resolution.width() == 800 && resolution.height() == 600) {
        resolution.setWidth(1024);
        resolution.setHeight(768);
    } else {
        resolution.setWidth(800);
        resolution.setHeight(600);
    }
}

bool Preferences::load(const QString &filename)
{
    /// Открытие файла
    QFile file(filename);
    /// Если файл не открылся, загрузка не происходит
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QXmlStreamReader xml(&file);
    /// Чтение xml файла
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        /// Значения из xml файла записываются в объект класса Preferences
        if (xml.isStartElement()) {
            /// Внешняя температура
            if (xml.name() == "TempVal")
                setTempVal(xml.readElementText().toDouble());
            /// Внешняя влажность
            else if (xml.name() == "HumidityVal")
                setHumidityVal(xml.readElementText().toDouble());
            /// Внешнее давление
            else if (xml.name() == "PressureVal")
                setPressureVal(xml.readElementText().toDouble());
            /// Единицы измерения температуры
            else if (xml.name() == "TempUnit")
                setTempUnit(xml.readElementText());
            /// Единицы измерения давления
            else if (xml.name() == "PressureUnit")
                setPressureUnit(xml.readElementText());
            /// Значение желаемой температуры
            else if (xml.name() == "TargetTemp")
                setTargetTemp(xml.readElementText().toDouble());
            /// Угол направления воздуха
            else if (xml.name() == "AcAngle")
                setAcAngle(xml.readElementText().toDouble());
            /// Ширина окна
            else if (xml.name() == "ResolutionW")
                resolution.setWidth(xml.readElementText().toInt());
            /// Высота окна
            else if (xml.name() == "ResolutionH")
                resolution.setHeight(xml.readElementText().toInt());
            /// Тёмная тема
            else if (xml.name() == "DarkTheme")
                darkTheme = (xml.readElementText() == "true");
            /// Значение питания кондиционера
            else if (xml.name() == "Power")
                power = (xml.readElementText() == "true");
        }
    }
    return !xml.hasError();
}

bool Preferences::save(const QString &filename) const
{
    /// Открытие файла
    QFile file(filename);
    /// Если файл не открылся, загрузка не происходит
    if (!file.open(QIODevice::WriteOnly))
        return false;    
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    /// Начало записи
    xml.writeStartDocument();
    /// Группа пользовательских настроек
    xml.writeStartElement("Preferences");
    /// Внешняя температура
    xml.writeTextElement("TempVal", QString::number(tempVal));
    /// Внешняя влажность
    xml.writeTextElement("HumidityVal", QString::number(humidityVal));
    /// Внешнее давление
    xml.writeTextElement("PressureVal", QString::number(pressureVal));
    /// Единицы измерения температуры
    xml.writeTextElement("TempUnit", QString(tempUnit));
    /// Единицы измерения давления
    xml.writeTextElement("PressureUnit", QString(pressureUnit));
    /// Значение желаемой температуры
    xml.writeTextElement("TargetTemp", QString::number(targetTemp));
    /// Угол направления воздуха
    xml.writeTextElement("AcAngle", QString::number(acAngle));
    /// Ширина окна
    xml.writeTextElement("ResolutionW", QString::number(resolution.width()));
    /// Высота окна
    xml.writeTextElement("ResolutionH", QString::number(resolution.height()));
    /// Темная тема
    xml.writeTextElement("DarkTheme", darkTheme ? "true" : "false");
    /// Питание
    xml.writeTextElement("Power", power ? "true" : "false");
    xml.writeEndElement();
    xml.writeEndDocument();
    return true;
}
