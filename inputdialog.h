/**
* @file
* @brief Заголовочный файл окна ввода значений
*
* Окно ввода состоит из трёх полей для ввода внешних значений.
*/
#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "preferences.h"

/**
 * @class InputDialog
 * @brief Окно для ввода данных
 *
 * Позволяет задать внешние показатели температуры, влажности и давления.
 */
class InputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputDialog();
    /**
     * @brief Запись значений в поля
     *
     * При открытии окна, в соответствующие поля автоматически заносятся значения температуры, влажности и давления.
     * @param prefs указатель на объект с пользовательскими настройками
     */
    void setValues(Preferences* prefs);
    /// @brief Геттер значения температуры
    qreal getTemp() const { return tempEdit->value(); }
    /// @brief Геттер значения влажности
    qreal getHumidity() const { return humidityEdit->value(); }
    /// @brief Геттер значения давления
    qreal getPressure() const { return pressureEdit->value(); }

private:    
    QVBoxLayout *mainLayout; ///< Основная компоновка

    QHBoxLayout *tempLayout; ///< Компоновка для температуры
    QLabel *tempLabel;  ///< Лейбл температуры
    QDoubleSpinBox *tempEdit; ///< Поле для ввода значения температуры

    QHBoxLayout *humidityLayout; ///< Компоновка для влажности
    QLabel *humidityLabel; ///< Лейбл влажности
    QDoubleSpinBox *humidityEdit; ///< Поле для ввода значения влажности

    QHBoxLayout *pressureLayout; ///< Компоновка для давления
    QLabel *pressureLabel; ///< Лейбл давления
    QDoubleSpinBox *pressureEdit; ///< Поле для ввода значения давления

    QHBoxLayout *buttonLayout; ///< Компоновка для кнопок
    QPushButton *okButton; ///< Кнопка "ОК"
    QPushButton *cancelButton; ///< Кнопка "Отмена"

    qreal temp;     ///< Значение поля температуры
    qreal pressure; ///< Значение поля влажности
    qreal humidity; ///< Значение поля давления

    /// @brief Инициализация окна ввода внешних значений
    void initDialog();
    /**
     * @brief Настройка минимальных и максимальных значений
     *
     * Задаются минимальные и максимальные значения для полей, в зависимости от текущих единиц измерения.
     * Температура варьируется от -40 до +50 по цельсию
     * Влажность варьируется от 0 до 100 процентов
     * Давление варьируется от 500 до 1000 мм рт. ст.
     *
     * @param prefs указатель на объект с пользовательскими настройками
     */
    void setMinMaxVals(Preferences* prefs);
    /// @brief Размещение элементов
    void placeItems();
};

#endif // INPUTDIALOG_H
