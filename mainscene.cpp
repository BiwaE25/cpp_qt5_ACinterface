#include "mainscene.h"
#include "preferences.h"
#include "inputdialog.h"
#include <QFont>
#include <QBrush>
#include <QColor>
#include <math.h>

CustomButton::CustomButton(const QString &text)
    : QPushButton(text)
{
    bName = text;
    style = styleSheet();
    connect(this, &QPushButton::pressed, this, &CustomButton::onPressed);
    connect(this, &QPushButton::released, this, &CustomButton::onReleased);
}

void CustomButton::applyButtonTheme(bool dark, bool power) {
    /// Стиль кнопки определяются двумя факторами: включен ли кондиционер и какая тема активна.
    /// Доступ к приватным константам MainScene осуществляется, потому что CustomButton - друг
    this->setStyleSheet(power ?
        (dark ? MainScene::ItemColor::CLR_buttonDark_ON : MainScene::ItemColor::CLR_buttonLight_ON) :
        (dark ? MainScene::ItemColor::CLR_buttonDark_OFF : MainScene::ItemColor::CLR_buttonLight_OFF)
    );
}

void CustomButton::onPressed() {
    /// Текущий стиль кнопки сохраняется
    style = styleSheet();
    setStyleSheet(MainScene::ItemColor::CLR_buttonPressed);
}

void CustomButton::onReleased() {
    /// Стиль кнопки откатывается к сохранённому
    setStyleSheet(style);
}

MainScene::MainScene(QObject *parent)
    : QGraphicsScene(parent),
    prefs(new Preferences)
{
    /// Загрузка свойств из xml файла
    loadPrefs();
    /// Построение графического интерфейса
    setUpUi();
    /// Применение темы
    applyTheme();
    /// Обновление надписей
    updateValues();
    /// Обновление позиций
    updatePos();
}

void MainScene::setUpUi(){
    /// Инициализация блоков с данными
    initAllBlocks();
    /// Размещение кнопок
    placeAllBlocks();

    /// Добавление в лист всех кнопок
    /// @{
    buttons.append(ui_changeTempUnit);
    buttons.append(ui_changePressureUnit);
    buttons.append(ui_tempMinusButton);
    buttons.append(ui_tempPlusButton);
    buttons.append(ui_inputButton);
    buttons.append(ui_resolutionButton);
    buttons.append(ui_themeButton);
    buttons.append(ui_powerButton);
    /// @}

    /// Привязка сигналов и слотов
    /// @{
    connect(ui_changeTempUnit, &QPushButton::clicked, this, &MainScene::changeTempUnit);
    connect(ui_changePressureUnit, &QPushButton::clicked, this, &MainScene::changePressureUnit);
    connect(ui_tempMinusButton, &QPushButton::clicked, this, &MainScene::onMinusTargetTemp);
    connect(ui_tempPlusButton, &QPushButton::clicked, this, &MainScene::onPlusTargetTemp);
    connect(ui_acAngleSlider, &QSlider::valueChanged, this, &MainScene::onSliderChanged);
    connect(ui_powerButton, &QPushButton::clicked, this, &MainScene::togglePower);
    connect(ui_resolutionButton, &QPushButton::clicked, this, &MainScene::changeResolution);
    connect(ui_themeButton, &QPushButton::clicked, this, &MainScene::changeTheme);
    connect(ui_inputButton, &QPushButton::clicked, this, &MainScene::openInputDialog);
    /// @}
}

void MainScene::initAllBlocks() {
    /// Инициализация шрифтов
    initFonts();
    /// Инициализация блока температуры
    initTemperatureBlock();
    /// Инициализация блока влажности
    initHumidityBlock();
    /// Инициализация блока давления
    initPressureBlock();
    /// Инициализация блока регулировки температуры
    initTargetTempBlock();
    /// Инициализация блока регулировки направления потока воздуха
    initAirDirectionBlock();
    /// Инициализация блока прочих функций
    initMiscButtons();
}

void MainScene::initFonts() {
    labelFont.setFamily("Arial");
    labelFont.setPointSize(14);
    valFont.setFamily("Arial");
    valFont.setPointSize(32);
}

void MainScene::initTemperatureBlock() {
    ui_tempLabel = new QGraphicsTextItem("ТЕМПЕРАТУРА");
    ui_tempLabel->setFont(labelFont);
    addItem(ui_tempLabel);

    ui_tempVal = new QGraphicsTextItem(QString::number(prefs->getTempVal(), 'f', 2));
    ui_tempVal->setFont(valFont);
    addItem(ui_tempVal);

    ui_tempUnitLabel = new QGraphicsTextItem(prefs->getTempUnit());
    ui_tempUnitLabel->setFont(labelFont);
    addItem(ui_tempUnitLabel);

    ui_changeTempUnit = new CustomButton("РЕЖИМ");
    ui_changeTempUnit->setFont(labelFont);
    ui_tempChangeUnitProxy = new QGraphicsProxyWidget;
    ui_tempChangeUnitProxy = addWidget(ui_changeTempUnit);
}

void MainScene::initHumidityBlock() {
    ui_humidityLabel = new QGraphicsTextItem("ВЛАЖНОСТЬ");
    ui_humidityLabel->setFont(labelFont);
    addItem(ui_humidityLabel);

    ui_humidityVal = new QGraphicsTextItem(QString::number(prefs->getHumidityVal(), 'f', 2));
    ui_humidityVal->setFont(valFont);
    addItem(ui_humidityVal);

    ui_humidityUnitLabel = new QGraphicsTextItem("%");
    ui_humidityUnitLabel->setFont(labelFont);
    addItem(ui_humidityUnitLabel);
}

void MainScene::initPressureBlock() {
    ui_pressureLabel = new QGraphicsTextItem("ДАВЛЕНИЕ");
    ui_pressureLabel->setFont(labelFont);
    addItem(ui_pressureLabel);

    ui_pressureVal = new QGraphicsTextItem(QString::number(prefs->getPressureVal(), 'f', 2));
    ui_pressureVal->setFont(valFont);
    addItem(ui_pressureVal);

    ui_pressureUnitLabel = new QGraphicsTextItem(prefs->getPressureUnit());
    ui_pressureUnitLabel->setFont(labelFont);
    addItem(ui_pressureUnitLabel);

    ui_changePressureUnit = new CustomButton("РЕЖИМ");
    ui_changePressureUnit->setFont(labelFont);
    ui_pressureChangeUnitProxy = new QGraphicsProxyWidget;
    ui_pressureChangeUnitProxy = addWidget(ui_changePressureUnit);
}

void MainScene::initTargetTempBlock() {
    ui_targetTempLabel = new QGraphicsTextItem("ЖЕЛАЕМАЯ ТЕМПЕРАТУРА:");
    ui_targetTempLabel->setFont(labelFont);
    addItem(ui_targetTempLabel);

    ui_targetTempVal = new QGraphicsTextItem(QString::number(prefs->getTargetTemp(), 'f', 2));
    ui_targetTempVal->setFont(valFont);
    addItem(ui_targetTempVal);

    ui_targetTempUnitLabel = new QGraphicsTextItem(prefs->getTempUnit());
    ui_targetTempUnitLabel->setFont(labelFont);
    addItem(ui_targetTempUnitLabel);

    ui_tempMinusButton = new CustomButton("-");
    ui_tempMinusButton->setFont(labelFont);
    ui_tempMinusButtonProxy = new QGraphicsProxyWidget;
    ui_tempMinusButtonProxy = addWidget(ui_tempMinusButton);
    ui_tempPlusButton = new CustomButton("+");
    ui_tempPlusButton->setFont(labelFont);
    ui_tempPlusButtonProxy = new QGraphicsProxyWidget;
    ui_tempPlusButtonProxy = addWidget(ui_tempPlusButton);
}

void MainScene::initAirDirectionBlock() {
    ui_acAngleLabel = new QGraphicsTextItem("НАПРАВЛЕНИЕ ВОЗДУХА:");
    ui_acAngleLabel->setFont(labelFont);
    addItem(ui_acAngleLabel);

    /// Вид слайдера - Горизонтальный
    ui_acAngleSlider = new QSlider(Qt::Horizontal);
    /// Диапазон значений слайдера
    /// @todo Убрать "магические" числа, если потребуется отображать значение угла
    ui_acAngleSlider->setRange(-15, 15);
    ui_acAngleSlider->setValue(prefs->getAcAngle());

    ui_acAngleSliderProxy = new QGraphicsProxyWidget;
    ui_acAngleSliderProxy = addWidget(ui_acAngleSlider);

    ui_acAngleDirection = new QGraphicsLineItem();
    /// Создаётся QPen для линии направления воздуха
    QPen pen(Qt::red);
    /// Ширина линии 8
    pen.setWidth(8);
    ui_acAngleDirection->setPen(pen);
    /// Линия располагается диагонально
    ui_acAngleDirection->setLine(50,50,150,150);
    /// Создаётся объект QTransform для вращения линии
    QTransform transform;
    /// Вращение линии при изменении положения слайдера
    transform.rotate(prefs->getAcAngle() * -1);
    /// Применение QTransform к линии
    ui_acAngleDirection->setTransform(transform);
    addItem(ui_acAngleDirection);

    acPolygon = new QPolygonF();
    ui_acBody = new QGraphicsPolygonItem(*acPolygon);

    ui_acBody->setPen(QPen(Qt::black,2));
    ui_acBody->setBrush(QBrush(ItemColor::CLR_acBodyLight_ON));
    addItem(ui_acBody);
}

void MainScene::initMiscButtons() {
    ui_inputButton = new CustomButton("Ввод значений");
    ui_inputButton->setFont(labelFont);
    ui_inputButtonProxy = new QGraphicsProxyWidget;
    ui_inputButtonProxy = addWidget(ui_inputButton);

    ui_resolutionButton = new CustomButton("Размер окна");
    ui_resolutionButton->setFont(labelFont);
    ui_resolutionButtonProxy = new QGraphicsProxyWidget;
    ui_resolutionButtonProxy = addWidget(ui_resolutionButton);

    ui_themeButton = new CustomButton("ТЕМА");
    ui_themeButton->setFont(labelFont);
    ui_themeButtonProxy = new QGraphicsProxyWidget;
    ui_themeButtonProxy = addWidget(ui_themeButton);

    /// Лейбл к кнопке питания, чтобы она стала более интуитивно понятной
    ui_powerButtonLabel = new QGraphicsTextItem("Кондиционер\n   Вкл/Выкл");
    ui_powerButtonLabel->setFont(labelFont);
    addItem(ui_powerButtonLabel);

    ui_powerButton = new CustomButton("I/O");
    ui_powerButton->setFont(valFont);
    ui_powerButtonProxy = new QGraphicsProxyWidget;
    ui_powerButtonProxy = addWidget(ui_powerButton);
}

/// @bug при установки разрешения 1024х768, элементы располагаются неровно, это видно по верхним трём блокам
void MainScene::placeAllBlocks() {
    QSize res = prefs->getResolution();
    /// Размер одной колонки определяется делением ширины окна на количество колонок
    oneColSize = res.width()/ItemPos::totalCols;
    /// Размер одной строки определяется делением высоты окна на количество строк
    oneRowSize = res.height()/ItemPos::totalRows;
    /// Размещение блока внешней температуры
    placeTemperatureBlock(res);
    /// Размещение блока внешней влажности
    placeHumidityBlock(res);
    /// Размещение блока внешнего давления
    placePressureBlock(res);
    /// Размещение блока регулировки температуры
    placeTargetTempBlock(res);
    /// Размещение блока регулировки направления потока воздуха
    placeAirDirectionBlock(res);
    /// Размещение блока прочих функций
    placeMiscButtons(res);
}

void MainScene::placeTemperatureBlock(const QSize &res) {
    placeItem(ui_tempLabel, ItemPos::COL_tempLabel, ItemPos::ROW_tempLabel, QSize(0,0), res);
    placeItem(ui_tempVal, ItemPos::COL_tempVal, ItemPos::ROW_tempVal, QSize(0,0), res);
    placeItem(ui_tempUnitLabel, ItemPos::COL_tempUnit, ItemPos::ROW_tempUnit, QSize(0,0), res);
    placeItem(ui_tempChangeUnitProxy, ItemPos::COL_tempChangeUnit, ItemPos::ROW_tempChangeUnit, QSize(oneColSize*16,oneRowSize*5), res);
}

void MainScene::placeHumidityBlock(const QSize &res) {
    placeItem(ui_humidityLabel, ItemPos::COL_humidityLabel, ItemPos::ROW_humidityLabel, QSize(0,0), res);
    placeItem(ui_humidityVal, ItemPos::COL_humidityVal, ItemPos::ROW_humidityVal, QSize(0,0), res);
    placeItem(ui_humidityUnitLabel, ItemPos::COL_humidityUnit, ItemPos::ROW_humidityUnit, QSize(0,0), res);
}

void MainScene::placePressureBlock(const QSize &res) {
    placeItem(ui_pressureLabel, ItemPos::COL_pressureLabel, ItemPos::ROW_pressureLabel, QSize(0,0), res);
    placeItem(ui_pressureVal, ItemPos::COL_pressureVal, ItemPos::ROW_pressureVal, QSize(0,0), res);
    placeItem(ui_pressureUnitLabel, ItemPos::COL_pressureUnit, ItemPos::ROW_pressureUnit, QSize(0,0), res);
    placeItem(ui_pressureChangeUnitProxy, ItemPos::COL_pressureChangeUnit, ItemPos::ROW_pressureChangeUnit, QSize(oneColSize*16,oneRowSize*5), res);
}

void MainScene::placeTargetTempBlock(const QSize &res) {
    placeItem(ui_targetTempLabel, ItemPos::COL_targetTempLabel, ItemPos::ROW_targetTempLabel, QSize(0,0), res);
    placeItem(ui_targetTempVal, ItemPos::COL_targetTempVal, ItemPos::ROW_targetTempVal, QSize(0,0), res);
    placeItem(ui_targetTempUnitLabel,ItemPos::COL_targetTempUnit, ItemPos::ROW_targetTempUnit, QSize(0,0), res);
    placeItem(ui_tempMinusButtonProxy, ItemPos::COL_targetTempMinusButton, ItemPos::ROW_targetTempMinusButton, QSize(oneColSize*5,oneRowSize*5), res);
    placeItem(ui_tempPlusButtonProxy, ItemPos::COL_targetTempPlusButton, ItemPos::ROW_targetTempPlusButton, QSize(oneColSize*5,oneRowSize*5), res);
}

void MainScene::placeAirDirectionBlock(const QSize &res) {
    placeItem(ui_acAngleLabel, ItemPos::COL_acAngleLabel, ItemPos::ROW_acAngleLabel, QSize(0,0), res);
    placeItem(ui_acAngleSliderProxy, ItemPos::COL_acAngleSlider, ItemPos::ROW_acAngleSlider, QSize(oneColSize*24,oneRowSize*5), res);
    placeItem(ui_acAngleDirection, ItemPos::COL_acAngleLine, ItemPos::ROW_acAngleLine, QSize(0,0), res);

    acPolygon->clear();
    acPolygon->append(QPointF(0, 0));
    acPolygon->append(QPointF(oneColSize*12, 0));
    acPolygon->append(QPointF(oneColSize*12, oneRowSize*6));
    acPolygon->append(QPointF(oneColSize*6, oneRowSize*12));
    acPolygon->append(QPointF(0, oneRowSize*12));
    ui_acBody->setPolygon(*acPolygon);

    placeItem(ui_acBody, ItemPos::COL_acBody, ItemPos::ROW_acBody, QSize(0,0), res);
}

void MainScene::placeMiscButtons(const QSize &res) {
    placeItem(ui_inputButtonProxy, ItemPos::COL_inputButton, ItemPos::ROW_inputButton, QSize(oneColSize*19,oneRowSize*5), res);
    placeItem(ui_resolutionButtonProxy, ItemPos::COL_resolutionButton, ItemPos::ROW_resolutionButton, QSize(oneColSize*19,oneRowSize*5), res);
    placeItem(ui_themeButtonProxy, ItemPos::COL_themeButton, ItemPos::ROW_themeButton, QSize(oneColSize*19,oneRowSize*5), res);
    placeItem(ui_powerButtonLabel, ItemPos::COL_powerButtonLabel, ItemPos::ROW_powerButtonLabel, QSize(0,0), res);
    placeItem(ui_powerButtonProxy, ItemPos::COL_powerButton, ItemPos::ROW_powerButton, QSize(oneColSize*10,oneRowSize*10), res);
}

void MainScene::placeItem(QGraphicsItem *item, qint16 col, qint16 row, QSize size, QSize res)
{
    /// Если размещается кнопка, то у неё устанавливается размер
    if (auto btn = dynamic_cast<QGraphicsProxyWidget*>(item))
    {
        btn->setMinimumSize(size);
        btn->setMaximumSize(size);
        btn->resize(size);
    }

    /// Область, занимаемая кнопкой
    QRectF rect = item->boundingRect();
    /// Инициализация "координат"
    qint16 x{0}, y{0};
    x = oneColSize * col - (rect.width() / 2);
    y = oneRowSize * row - (rect.height() / 2);
    item->setPos(x, y);
}

void MainScene::togglePower()
{
    /// Переключение питания
    prefs->setPower();
    /// Применение темы
    applyTheme();
}

void MainScene::updatePos() {
    placeAllBlocks();
}


void MainScene::changeTempUnit()
{
    qreal targetT = prefs->getTargetTemp();
    qreal temp = prefs->getTempVal();

    /// Из Кельвинов в Цельсии
    if (prefs->getTempUnit() == "°K") {
        temp -= 273.15;
        targetT -= 273.15;
        prefs->setTempUnit("°C");        
    } /// Из Цельсиев в Фаренгейты
    else if (prefs->getTempUnit() == "°C") {
        temp = (temp * 9 / 5) + 32;
        targetT = (targetT * 9 / 5) + 32;
        prefs->setTempUnit("°F");
    } /// Из Фаренгейтов в Кельвины
    else {
        temp = (temp - 32) * 5 / 9 + 273.15;
        targetT = (targetT  - 32) * 5 / 9 + 273.15;
        prefs->setTempUnit("°K");
    }
    prefs->setTempVal(temp);
    prefs->setTargetTemp(targetT);
    prefs->setLimits();
    updateValues();
    updatePos();
}

void MainScene::changePressureUnit()
{
    qreal pressure = prefs->getPressureVal();
    /// Из Миллиметров в Паскали
    if (prefs->getPressureUnit() == "мм") {
        pressure *= 133.322;
        prefs->setPressureUnit("Pa");
    } /// Из Паскалей в Миллиметры
    else {
        pressure /= 133.322;
        prefs->setPressureUnit("мм");
    }

    prefs->setPressureVal(pressure);
    prefs->setLimits();
    updateValues();
    updatePos();
}

void MainScene::changeResolution()
{
    prefs->setResolution();
    QSize res = prefs->getResolution();
    updatePos();
    /// Активация сигнала, по которой сработает обработчик в main.cpp
    emit resolutionChanged(res);
}

void MainScene::changeTheme()
{
    prefs->setTheme();
    applyTheme();
}

void MainScene::applyTheme()
{
    bool dark = prefs->getTheme();
    bool power = prefs->getPower();
    /// Установка цвета фона
    setBackgroundBrush(QBrush(dark ? ItemColor::CLR_bgDark : ItemColor::CLR_bgLight));

    /// Установка цвета текста
    for (QGraphicsItem* item : items()) {
        if (auto text = dynamic_cast<QGraphicsTextItem*>(item))
            text->setDefaultTextColor(power ?
                (dark ? ItemColor::CLR_textDark_ON :ItemColor::CLR_textLight_ON) :
                (dark ? ItemColor::CLR_textDark_OFF :ItemColor::CLR_textLight_OFF)
            );
    }

    /// Установка цвета кнопок
    for (auto x : buttons) {
        /// Кнопка питания всегда доступна
        if (x->bName != "I/O") {
            x->setEnabled(power);
            x->applyButtonTheme(dark, power);
        } else {
            x->applyButtonTheme(dark, true);
        }
    }
    /// Цвет текста на кнопке питания должет быть "включенным"
    ui_powerButtonLabel->setDefaultTextColor((dark ? ItemColor::CLR_textDark_ON :ItemColor::CLR_textLight_ON));

    ui_acAngleSlider->setEnabled(power);
    /// Установка цвета слайдера
    ui_acAngleSlider->setStyleSheet(power ?
        (dark ? ItemColor::CLR_sliderDark_ON : ItemColor::CLR_sliderLight_ON) :
        (dark ? ItemColor::CLR_sliderDark_OFF : ItemColor::CLR_sliderLight_OFF)
    );
    /// Установка цвета полигона
    ui_acBody->setBrush(power ?
        (dark ? ItemColor::CLR_acBodyDark_ON : ItemColor::CLR_acBodyLight_ON) :
        (dark ? ItemColor::CLR_acBodyDark_OFF : ItemColor::CLR_acBodyLight_OFF)
    );
    /// Установка цвета рамки полигона
    QPen penBody(power ?
        (dark ? ItemColor::CLR_borderDark_ON : ItemColor::CLR_borderLight_ON) :
        (dark ? ItemColor::CLR_borderDark_OFF : ItemColor::CLR_borderLight_OFF)
    );
    penBody.setWidth(2);
    ui_acBody->setPen(penBody);
    /// Установка цвета линии направления воздуха
    QPen penLine(power ?
        (dark ? ItemColor::CLR_acLineDark_ON : ItemColor::CLR_acLineLight_ON) :
        (dark ? ItemColor::CLR_acLineDark_OFF : ItemColor::CLR_acLineLight_OFF)
    );
    penLine.setWidth(8);
    ui_acAngleDirection->setPen(penLine);
}

void MainScene::updateValues() {
    ui_tempVal->setPlainText(QString::number(prefs->getTempVal(), 'f', 2));
    ui_humidityVal->setPlainText(QString::number(prefs->getHumidityVal(), 'f', 2));
    ui_pressureVal->setPlainText(QString::number(prefs->getPressureVal(), 'f', 2));
    ui_tempUnitLabel->setPlainText(prefs->getTempUnit());
    ui_pressureUnitLabel->setPlainText(prefs->getPressureUnit());
    ui_targetTempVal->setPlainText(QString::number(prefs->getTargetTemp(), 'f', 2));
    ui_targetTempUnitLabel->setPlainText(prefs->getTempUnit());
}

void MainScene::onMinusTargetTemp()
{
    qreal temp = prefs->getTargetTemp();
    qreal stepVal = roundTargetTemp();
    qreal tempMin = prefs->getTempMin();
    /// Если значение температуры кратно шагу
    if (stepVal != 0) {
        /// Если значение температуры отрицательное, нужно "инвертировать" недостающее до кратного число
        if (temp < 0)
            stepVal = TEMPSTEP - stepVal;
    } else
        stepVal = TEMPSTEP;
    /// Проверка на выход за границы минимального значения
    if (temp - stepVal < tempMin)
        prefs->setTargetTemp(tempMin);
    else
        prefs->setTargetTemp(temp - stepVal);

    updateValues();
    updatePos();
}

void MainScene::onPlusTargetTemp()
{
    qreal temp = prefs->getTargetTemp();
    qreal stepVal = roundTargetTemp();
    qreal tempMax = prefs->getTempMax();
    /// Если значение температуры кратно шагу
    if (stepVal != 0) {
        /// Если значение температуры отрицательное, нужно "инвертировать" недостающее до кратного число
        if (temp > 0)
            stepVal = TEMPSTEP - stepVal;
    } else
        stepVal = TEMPSTEP;
    /// Проверка на выход за границы максимального значения
    if (temp + stepVal > tempMax)
        prefs->setTargetTemp(tempMax);
    else
        prefs->setTargetTemp(temp + stepVal);

    updateValues();
    updatePos();
}

qreal MainScene::roundTargetTemp()
{
    /// Вычисление модуля температуры
    qreal temp = std::fabs(prefs->getTargetTemp());
    /// Определение шага изменения температуры
    qreal calcStep = TEMPSTEP * 100;
    /// Определение дробной части в виде целого числа
    qint32 rem = static_cast<qint32>(std::round(temp * 100));
    /// Сколько не хватает до числа, кратного шагу изменения температуры
    rem %= (static_cast<qint32>(calcStep));
    /// Если значение не кратно
    if (rem != 0)
        calcStep = rem / 100.00;
    else /// Если значение кратно
        calcStep = 0;
    /// Метод возвращает, сколько не хватает до следующего кратного значения
    return calcStep;
}

void MainScene::onSliderChanged(int value)
{
    /// Создаётся объект QTransform для вращения линии
    QTransform transform;
    /// Вращение линии при изменении положения слайдера
    transform.rotate(value * -1);
    prefs->setAcAngle(value);
    /// Применение QTransform к линии
    ui_acAngleDirection->setTransform(transform);
}

void MainScene::loadPrefs()
{
    prefs->load("preferences.xml");
}

void MainScene::savePrefs()
{    
    prefs->save("preferences.xml");    
}
