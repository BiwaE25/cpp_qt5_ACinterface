/**
* @file
* @brief Заголовочный файл пользовательских настроек
*
* Пользовательские настройки содержат все данные, используемые для работы кондиционера.
* Внешние данные температуры, влажности и давление;
* Единицы измерения температуры, влажности и давления;
* Значение и единица измерения желаемой температуры;
* Максимальные и минимальные значения температуры, влажности и давления;
* Угол подачи воздуха;
* Разрешение окна;
* Включена ли тёмная тема;
* Включен ли кондиционер;
*/
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QSize>
#include <utility>

/**
 * @class Preferences
 * @brief Класс пользовательских настроек
 */
class Preferences
{
public:
    Preferences();
    /**
     * @brief Загрузка параметров из XML-файла
     *
     * Файл считывается построчно и записывает данные в соответствующие свойства
     * Загрузка происходит при запуске программы
     * @param filename Путь к файлу
     * @return true если загрузка прошла успешно
     */
    bool load(const QString &filename);
    /**
     * @brief Сохранение параметров в XML-файл
     *
     * Сохранение происходит при закрытии программы
     * @param filename Путь к файлу
     * @return true если загрузка прошла успешно
     */
    bool save(const QString &filename) const;
    /// @brief Обновление максимальных/минимальных значений в зависимости от текущих единиц измерения
    void setLimits();

    /// @brief Геттер значения темы
    bool getTheme() const { return darkTheme; }
    /// @brief Сеттер значения темы
    void setTheme() {darkTheme = !darkTheme; }
    /// @brief Геттер значения питания кондиционера
    bool getPower() const { return power; }
    /// @brief Сеттер значения питания кондиционера
    void setPower() { power = !power; }
    /// @brief Геттер единицы измерения температуры
    QString getTempUnit() const { return tempUnit; }
    /// @brief Сеттер единицы измерения температуры
    void setTempUnit(QString val) { tempUnit = val; }
    /// @brief Геттер единицы измерения давления
    QString getPressureUnit() const { return pressureUnit; }
    /// @brief Сеттер единицы измерения давления
    void setPressureUnit(QString val) { pressureUnit = val; }
    /// @brief Геттер значения желаемой температуры
    qreal getTargetTemp() const { return targetTemp; }
    /// @brief Сеттер значения желаемой температуры
    void setTargetTemp(qreal val) { targetTemp = val; }
    /// @brief Геттер значения разрешения
    QSize getResolution() const { return resolution; }
    /// @brief Сеттер значения разрешения. Переключается между двумя режимами.
    void setResolution();
    /// @brief Геттер значения внешней температуры
    qreal getTempVal() const { return tempVal; }
    /// @brief Сеттер значения внешней температуры
    void setTempVal(qreal val) { tempVal = val; }
    /// @brief Геттер значения внешней влажности
    qreal getHumidityVal() const { return humidityVal; }
    /// @brief Сеттер значения внешней влажности
    void setHumidityVal(qreal val) { humidityVal = val; }
    /// @brief Геттер значения внешнего давления
    qreal getPressureVal() const { return pressureVal; }
    /// @brief Сеттер значения внешнего давления
    void setPressureVal(qreal val) { pressureVal = val; }
    /// @brief Геттер значения угла направления воздуха
    qreal getAcAngle() const { return acAngle; }
    /// @brief Сеттер значения угла направления воздуха
    void setAcAngle(qreal val) { acAngle = val; }
    /// @brief Геттер минимального значения температуры
    qreal getTempMin() const { return tempMin; }
    /// @brief Геттер максимального значения температуры
    qreal getTempMax() const { return tempMax; }
    /// @brief Геттер минимального значения влажности
    qreal getHumidityMin() const { return humidityMin; }
    /// @brief Геттер максимального значения влажности
    qreal getHumidityMax() const { return humidityMax; }
    /// @brief Геттер минимального значения давления
    qreal getPressureMin() const { return pressureMin; }
    /// @brief Геттер максимального значения давления
    qreal getPressureMax() const { return pressureMax; }

private:
    /// Значение внешней температуры
    qreal tempVal;
    /// Значение внешней влажности
    qreal humidityVal;
    /// Значение внешнего давления
    qreal pressureVal;
    /// Минимальное значение температуры
    qreal tempMin;
    /// Максимальное значение температуры
    qreal tempMax;
    /// Минимальное значение влажности
    qreal humidityMin;
    /// Максимальное значение влажности
    qreal humidityMax;
    /// Минимальное значение давления
    qreal pressureMin;
    /// Максимальное значение давления
    qreal pressureMax;
    /// Единица измерения температуры
    QString tempUnit;
    /// Единицы измерения давления
    QString pressureUnit;
    /// Значение желаемой температуры
    qreal targetTemp;
    /// Разрешение главного окна
    QSize resolution;
    /// Угол направления воздуха
    qreal acAngle;
    /// Включена ли тёмная тема
    bool darkTheme;
    /// Включен ли кондиционер
    bool power;
    /// Инициализация всех значений этого класса
    void initValues();
};

#endif // PREFERENCES_H
