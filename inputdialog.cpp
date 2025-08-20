#include "inputdialog.h"
#include "preferences.h"

InputDialog::InputDialog() : QDialog()
{
    /// Инициализация окна
    initDialog();
    /// Размещение элементов
    placeItems();
    /// Привязка кнопки "ОК" к стандартному методу QDialog::accept
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    /// Привязка кнопки "Отмена" к стандартному методу QDialog::reject
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void InputDialog::initDialog()
{
    /// Текст в шапке окна
    setWindowTitle("Ввод данных");
    /// Инициализация компоновки для температуры
    tempLayout = new QHBoxLayout();
    /// Инициализация лейбла "Температура"
    tempLabel = new QLabel("Температура");
    /// Инициализация поля для ввода температуры
    tempEdit = new QDoubleSpinBox();
    /// Инициализация компоновки для влажности
    humidityLayout = new QHBoxLayout();
    /// Инициализация лейбла "Влажность"
    humidityLabel = new QLabel("Влажность (%)");
    /// Инициализация поля для ввода влажности
    humidityEdit = new QDoubleSpinBox();
    /// Инициализация компоновки для давления
    pressureLayout = new QHBoxLayout();
    /// Инициализация лейбла "Давление"
    pressureLabel = new QLabel("Давление (Pa)");
    /// Инициализация поля для ввода давления
    pressureEdit = new QDoubleSpinBox();
    /// Инициализация компоновки для кнопок
    buttonLayout = new QHBoxLayout();
    /// Инициализация кнопки "ОК"
    okButton = new QPushButton("Ок");
    /// Инициализация кнопки "Отмена"
    cancelButton = new QPushButton("Отмена");
    /// Инициализация главной компоновки
    mainLayout = new QVBoxLayout(this);
}

void InputDialog::setMinMaxVals(Preferences* prefs)
{
    /// Минимальная температура
    tempEdit->setMinimum(prefs->getTempMin());
    /// Максимальная температура
    tempEdit->setMaximum(prefs->getTempMax());
    /// Минимальная влажность
    humidityEdit->setMinimum(prefs->getHumidityMin());
    /// Максимальная влажность
    humidityEdit->setMaximum(prefs->getHumidityMax());
    /// Минимальное давление
    pressureEdit->setMinimum(prefs->getPressureMin());
    /// Максимальное давление
    pressureEdit->setMaximum(prefs->getPressureMax());
}
void InputDialog::placeItems()
{
    /// Добавление лейбла температуры
    tempLayout->addWidget(tempLabel);
    /// Добавление поля для ввода температуры
    tempLayout->addWidget(tempEdit);
    /// Добавление компоновки температуры на главную компоновку
    mainLayout->addLayout(tempLayout);

    /// Добавление лейбла влажности
    humidityLayout->addWidget(humidityLabel);
    /// Добавление поля для ввода влажности
    humidityLayout->addWidget(humidityEdit);
    /// Добавление компоновки влажности на главную компоновку
    mainLayout->addLayout(humidityLayout);

    /// Добавление лейбла давления
    pressureLayout->addWidget(pressureLabel);
    /// Добавление поля для ввода давления
    pressureLayout->addWidget(pressureEdit);
    /// Добавление компоновки давления на главную компоновку
    mainLayout->addLayout(pressureLayout);

    /// Добавление кнопки ОК
    buttonLayout->addWidget(okButton);
    /// Добавление кнопки Отмена
    buttonLayout->addWidget(cancelButton);
    /// Добавление компоновки кнопок на главную компоновку
    mainLayout->addLayout(buttonLayout);
}

void InputDialog::setValues(Preferences* prefs)
{
    /// Настройка минимальных и максимальных значений
    setMinMaxVals(prefs);

    /// Лейбл "Температура" с текущей ЕИ
    tempLabel->setText(QString("Температура (%1)").arg(prefs->getTempUnit()));
    /// Задаётся текущее значение внешней температуры
    tempEdit->setValue(prefs->getTempVal());

    /// Лейбл "Влажность" не меняется
    /// Задаётся текущее значение внешней влажности
    humidityEdit->setValue(prefs->getHumidityVal());

    /// Лейбл "Давление" с текущей ЕИ
    pressureLabel->setText(QString("Давление (%1)").arg(prefs->getPressureUnit()));
    /// Задаётся текущее значение внешнего давления
    pressureEdit->setValue(prefs->getPressureVal());
}
