/**
* @file
* @brief Заголовочный файл интерфейса
*
* Содержит в себе всё, что касается интерфейса.
*/

#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QSlider>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include "preferences.h"
/**
 * @class CustomButton
 * @brief Класс для кнопок
 *
 * Класс, наследованный от QPushButton, добавляющий функции для смены темы.
 */
class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomButton(const QString &text);
    /// Наименование кнопки, содержит в себе текст, отображаемный на кнопке
    QString bName;
    /// Текущий стиль (используется для setStyleSheet)
    QString style;
    /**
     * @brief Применение цвета текущей темы
     * @param dark тёмная тема
     * @param power питание
     */
    void applyButtonTheme(bool dark, bool power);

private slots:
    /// @brief Обработчик при нажатии на кнопку
    void onPressed();
    /// @brief Обработчик при отпускании кнопки
    void onReleased();
};

/**
 * @class MainScene
 * @brief Основной интерфейс
 *
 * Специальный класс на основе QGraphicScene, на котором находятся все элементы интерфейса
 */
class MainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MainScene(QObject *parent = nullptr);
    /// Объект с пользовательскими настройками
    Preferences *prefs;
    /**
     * @brief Инициализация интерфейса
     *
     * Инициализирует и размещает все элементы интерфейса
     */
    void setUpUi();    
    /// @brief Обновление отображаемых данных
    void updateValues();
    /// @brief Обновление позиции элементов интерфейса
    void updatePos();
    /// @brief Вызов загрузки xml файла
    void loadPrefs();
    /// @brief Вызов сохранения xml файла
    void savePrefs();
    /// Класс CustomButton - друг. Нужно для использования значений цветов.
    friend class CustomButton;
private:
    /// Значение шага изменения желаемой температуры
    static constexpr qreal TEMPSTEP = 0.5;
    /// Шрифт для лейблов и кнопок
    QFont labelFont;
    /// Шрифт для значений
    QFont valFont;

    // Лист кнопок, используется при применении темы к кнопкам
    QList<CustomButton*> buttons;

    /**
     * @defgroup uiPlace Размещение элементов интерфейса
     * @brief Окно делится на условные колонки и строки, элементы размещаются по координатам (колонка,строка)
     */
    /// @{
    /// Ширина одной колонки
    qint16 oneColSize;
    /// Ширина одной строки
    qint16 oneRowSize;
    /// @}

    /**
     * @defgroup tempBlock Блок внешних данных : Температура
     * @brief Показывает данные о внешней температуре
     */
    /// @{
    /// Лейбл "Температура"
    QGraphicsTextItem *ui_tempLabel;
    /// Значение температуры
    QGraphicsTextItem *ui_tempVal;
    /// Единица измерения температуры
    QGraphicsTextItem *ui_tempUnitLabel;
    /// Кнопка смены единиц измерения температуры (действует и для желаемой температуры)
    CustomButton *ui_changeTempUnit;
    /// Прокси-виджет для кнопки смены ЕИ температуры, чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_tempChangeUnitProxy;
    /// @}

    /**
     * @defgroup humidityBlock Блок внешних данных : Влажность
     * @brief Показывает данные о внешней влажности
     */
    /// @{
    /// Лейбл "Влажность"
    QGraphicsTextItem *ui_humidityLabel;
    /// Значение влажности
    QGraphicsTextItem *ui_humidityVal;
    /// Единица измерения влажности
    QGraphicsTextItem *ui_humidityUnitLabel;
    /// @}

    /**
     * @defgroup pressureBlock Блок внешних данных : Давление
     * @brief Показывает данные о внешнем давлении
     */
    /// @{
    /// Лейбл "Давление"
    QGraphicsTextItem *ui_pressureLabel;
    /// Значение давления
    QGraphicsTextItem *ui_pressureVal;
    /// Единица измерения давления
    QGraphicsTextItem *ui_pressureUnitLabel;
    /// Кнопка смены единиц измерения давления
    CustomButton *ui_changePressureUnit;
    /// Прокси-виджет для кнопки смены ЕИ давления, чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_pressureChangeUnitProxy;
    /// @}

    /**
     * @defgroup targetTempBlock Блок управления температурой
     * @brief Здесь отображается текущая желаемая температура, а также есть 2 элемента управления: кнопки "-" и "+"
     */
    /// @{
    /// Лейбл "Желаемая температура"
    QGraphicsTextItem *ui_targetTempLabel;
    /// Значение желаемой температуры
    QGraphicsTextItem *ui_targetTempVal;
    /// Единица измерения желаемой температуры (синхронизирована с ЕИ внешней температуры)
    QGraphicsTextItem *ui_targetTempUnitLabel;
    /// Кнопка "-" для уменьшения желаемой температуры
    CustomButton *ui_tempMinusButton;
    /// Прокси-виджет для кнопки "-", чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_tempMinusButtonProxy;
    /// Кнопка "+" для увеличения желаемой температуры
    CustomButton *ui_tempPlusButton;
    /// Прокси-виджет для кнопки "+", чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_tempPlusButtonProxy;
    /**
     * @brief Округление значения желаемой температуры
     * @return Разница с ближайшим значением, которое кратно шагу изменения температуры
     *
     * Округление желаемой температуры к значению, кратному шагу изменения желаемой температуры.
     * Шаг по умолчанию равен 0,5. При смене единиц измерения, дробная часть желаемой температуры не кратна этому значению.
     * При изменении желаемой температуры вызовется этот метод и всё починит.
     */
    qreal roundTargetTemp();
    /// @}

    /**
     * @defgroup acBlock Блок управления направлением потока воздуха
     * @brief Состоит из слайдера и визуализации направления воздуха
     */
    /// @{
    /// Лейбл "Направление воздуха"
    QGraphicsTextItem *ui_acAngleLabel;
    /// Слайдер для регулировки направления воздуха
    QSlider *ui_acAngleSlider;
    /// Прокси-виджет для расположения слайдера на QGraphicsScene
    QGraphicsProxyWidget *ui_acAngleSliderProxy;
    /// Линия, показывающая направление воздуха
    QGraphicsLineItem *ui_acAngleDirection;
    /// Объект на основе полигона, показывающий корпус кондиционера, из которого выходит линия направления воздуха
    QGraphicsPolygonItem *ui_acBody;
    /// Полигон, на основе которого строится визуализация корпуса кондиционера
    QPolygonF *acPolygon;
    /// @}

    /**
     * @defgroup other Блок прочих функций
     * @brief Состоит из кнопок: ввод внешних данных, смена разрешения, смена темы, вкл/выкл кондиционер
     */
    /// @{
    /// Кнопка "Ввод внешних данных"
    CustomButton *ui_inputButton;
    /// Прокси-виджет для кнопки ввода внешних данных, чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_inputButtonProxy;

    /// Кнопка "Смена разрешения"
    CustomButton *ui_resolutionButton;
    /// Прокси-виджет для кнопки смены разрешения, чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_resolutionButtonProxy;

    /// Кнопка "Смена темы"
    CustomButton *ui_themeButton;
    /// Прокси-виджет для кнопки смены темы, чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_themeButtonProxy;

    /// Лейбл кнопки питания
    QGraphicsTextItem *ui_powerButtonLabel;
    /// Кнопка "Вкл/выкл"
    CustomButton *ui_powerButton;
    /// Прокси-виджет для кнопки питания кондиционера, чтобы её можно было разместить на QGraphicsScene
    QGraphicsProxyWidget *ui_powerButtonProxy;
    /// @}

    /**
     * @defgroup initUi Инициализация элементов интерфейса
     * @brief Методы инициализации групп элементов
     */
    /// @brief Инициализация блоков внешних данных
    void initAllBlocks();
    /// @brief Инициализация шрифтов
    void initFonts();
    /// @brief Инициализация блока внешней температуры
    void initTemperatureBlock();
    /// @brief Инициализация блока внешней влажности
    void initHumidityBlock();
    /// @brief Инициализация блока внешнего давления
    void initPressureBlock();
    /// @brief Инициализация блока регулировки температуры
    void initTargetTempBlock();
    /// @brief Инициализация блока регулировки направления потока воздуха
    void initAirDirectionBlock();
    /// @brief Инициализация блока общих настроек системы
    void initMiscButtons();
    /**
     * @defgroup placeMethods
     * @ingroup uiPlace
     * @brief Методы размещения элементов интерфейса
     */
    /// @{
    /**
     * @brief Размещение одного элемента
     * @param item Размещаемый элемент
     * @param col Условная колонка
     * @param row Условная строка
     * @param size Размер элемента (Для текста указывается QSize(0,0))
     * @param res Разрешение окна
     *
     * Размещение конкретного элемента в конкретном месте экрана, определяемом по координатам (колонка,строка).
     */
    void placeItem(QGraphicsItem *item, qint16 col, qint16 row, QSize size, QSize res);
    /// @brief Размещение элементов
    void placeAllBlocks();
    /// @brief Размещение блока внешней температуры
    void placeTemperatureBlock(const QSize &res);
    /// @brief Размещение блока внешней влажности
    void placeHumidityBlock(const QSize &res);
    /// @brief Размещение блока внешнего давления
    void placePressureBlock(const QSize &res);
    /// @brief Размещение блока регулировки температуры
    void placeTargetTempBlock(const QSize &res);
    /// @brief Размещение блока регулировки направления воздуха
    void placeAirDirectionBlock(const QSize &res);
    /// @brief Размещение кнопок для регулировки температуры
    void placeMiscButtons(const QSize &res);
    /// @}

    /// @brief Применение темы
    void applyTheme();

    /// @defgroup itemPos Константы с координатами и размеров элементов интерфейса
    /// @{
    struct ItemPos {
        /// Общее количество колонок
        static const int totalCols = 80;
        /// Общее количество строк
        static const int totalRows = 60;

        /// Расположение элементов блока внешней температуры
        /// @ingroup tempBlock
        /// @{
        static const int COL_tempLabel = 14;       static const int ROW_tempLabel = 4;
        static const int COL_tempVal = 14;         static const int ROW_tempVal = 9;
        static const int COL_tempUnit = 14;        static const int ROW_tempUnit = 13;
        static const int COL_tempChangeUnit = 14;  static const int ROW_tempChangeUnit = 18;
        /// @}

        /// Расположение элементов блока внешней влажности
        /// @ingroup humidityBlock
        /// @{
        static const int COL_humidityLabel = 40; static const int ROW_humidityLabel = 4;
        static const int COL_humidityVal = 40;   static const int ROW_humidityVal = 9;
        static const int COL_humidityUnit = 40;  static const int ROW_humidityUnit = 13;
        /// @}

        /// Расположение элементов блока внешнего давления
        /// @ingroup pressureBlock
        /// @{
        static const int COL_pressureLabel = 66;       static const int ROW_pressureLabel = 4;
        static const int COL_pressureVal = 66;         static const int ROW_pressureVal = 9;
        static const int COL_pressureUnit = 66;        static const int ROW_pressureUnit = 13;
        static const int COL_pressureChangeUnit = 66;  static const int ROW_pressureChangeUnit = 18;
        /// @}

        /// Расположение элементов блока регулировки температуры
        /// @ingroup targetTempBlock
        /// @{
        static const int COL_targetTempLabel = 20;       static const int ROW_targetTempLabel = 26;
        static const int COL_targetTempVal = 20;         static const int ROW_targetTempVal = 31;
        static const int COL_targetTempUnit = 20;        static const int ROW_targetTempUnit = 35;
        static const int COL_targetTempMinusButton = 8; static const int ROW_targetTempMinusButton = 31;
        static const int COL_targetTempPlusButton = 32;  static const int ROW_targetTempPlusButton = 31;
        /// @}

        /// Расположение элементов блока управления потоком воздуха
        /// @ingroup acBlock
        /// @{
        static const int COL_acAngleLabel = 62;  static const int ROW_acAngleLabel = 26;
        static const int COL_acAngleSlider = 62; static const int ROW_acAngleSlider = 31;
        static const int COL_acAngleLine = 62;   static const int ROW_acAngleLine = 46;
        static const int COL_acBody = 62;        static const int ROW_acBody = 46;
        /// @}

        /// Расположение элементов блока прочих функций
        /// @ingroup other
        /// @{
        static const int COL_inputButton = 15;      static const int ROW_inputButton = 42;
        static const int COL_resolutionButton = 15; static const int ROW_resolutionButton = 48;
        static const int COL_themeButton = 15;      static const int ROW_themeButton = 54;
        static const int COL_powerButtonLabel = 40; static const int ROW_powerButtonLabel = 42;
        static const int COL_powerButton = 40;      static const int ROW_powerButton = 51;
        /// @}
    };
    /// @}

    /// @defgroup itemColor Константы со значениями цвета элементов
    /// @{
    struct ItemColor {
        /// Цвет фона - светлая тема
        static constexpr QColor CLR_bgLight = QColor(230,230,230);
        /// Цвет фона - темная тема
        static constexpr QColor CLR_bgDark = QColor(40,40,40);

        /// Цвет текста - ВКЛ - светлая тема
        static constexpr QColor CLR_textLight_ON = QColor(10,10,10);
        /// Цвет текста - ВКЛ - темная тема
        static constexpr QColor CLR_textDark_ON = QColor(200,200,200);
        /// Цвет текста - ВЫКЛ - светлая тема
        static constexpr QColor CLR_textLight_OFF = QColor(180,180,180);
        /// Цвет текста - ВЫКЛ - темная тема
        static constexpr QColor CLR_textDark_OFF = QColor(60,60,60);

        /// Цвет рамки - ВКЛ - светлая тема
        static constexpr QColor CLR_borderLight_ON = CLR_textLight_ON;
        /// Цвет рамки - ВКЛ - темная тема
        static constexpr QColor CLR_borderDark_ON = CLR_textDark_OFF;
        /// Цвет рамки - ВЫКЛ - светлая тема
        static constexpr QColor CLR_borderLight_OFF = CLR_textLight_OFF;
        /// Цвет рамки - ВЫКЛ - темная тема
        static constexpr QColor CLR_borderDark_OFF = CLR_borderDark_ON;

        /// Цвет кнопки - ВКЛ - светлая тема
        inline static const QString CLR_buttonLight_ON = R"(
            background-color: rgb(150,150,150);
            color: rgb(10,10,10);
            border: 2px solid;
            border-color: rgb(60,60,60);
        )";
        /// Цвет кнопки - ВКЛ - темная тема
        inline static const QString CLR_buttonDark_ON = R"(
            background-color: rgb(60,60,60);
            color: rgb(245,245,245);
            border: 2px solid;
            border-color: rgb(180,180,180);
        )";
        /// Цвет кнопки - ВЫКЛ - светлая тема
        inline static const QString CLR_buttonLight_OFF = R"(
            background-color: rgb(210,210,210);
            color: rgb(180,180,180);
            border: 2px solid;
            border-color: rgb(210,210,210);
        )";
        /// Цвет кнопки - ВЫКЛ - темная тема
        inline static const QString CLR_buttonDark_OFF = R"(
            background-color: rgb(30,30,30);
            color: rgb(60,60,60);
            border: 2px solid;
            border-color: rgb(30,30,30);
        )";

        /// Цвет кнопки - при нажатии
        inline static const QString CLR_buttonPressed = "background-color: rgb(180,180,180);";

        /// Цвет слайдера - ВКЛ - светлая тема
        inline static const QString CLR_sliderLight_ON = R"(
            QSlider::groove:horizontal { background: rgb(150,150,150); border: 2px solid; border-color: rgb(10,10,10);}
            QSlider::handle:horizontal { background: rgb(80,80,80); width: 50px;}
        )";
        /// Цвет слайдера - ВКЛ - темная тема
        inline static const QString CLR_sliderDark_ON = R"(
            QSlider::groove:horizontal { background: rgb(100,100,100); border: 2px solid; border-color: rgb(180,180,180);}
            QSlider::handle:horizontal { background: rgb(200,200,200); width: 50px; }
        )";
        /// Цвет слайдера - ВЫКЛ - светлая тема
        inline static const QString CLR_sliderLight_OFF = R"(
            QSlider::groove:horizontal { background: rgb(200,200,200); }
            QSlider::handle:horizontal { background: rgb(150,150,150); width: 50px; }
        )";
        /// Цвет слайдера - ВЫКЛ - темная тема
        inline static const QString CLR_sliderDark_OFF = R"(
            QSlider::groove:horizontal { background: rgb(50,50,50); }
            QSlider::handle:horizontal { background: rgb(100,100,100); width: 50px; }
        )";

        /// Цвет полигона - ВКЛ - светлая тема
        static constexpr QColor CLR_acBodyLight_ON = QColor(150,150,150);
        /// Цвет полигона - ВКЛ - темная тема
        static constexpr QColor CLR_acBodyDark_ON = QColor(200,200,200);
        /// Цвет полигона - ВЫКЛ - светлая тема
        static constexpr QColor CLR_acBodyLight_OFF = QColor(200,200,200);
        /// Цвет полигона - ВЫКЛ - темная тема
        static constexpr QColor CLR_acBodyDark_OFF = QColor(50,50,50);

        /// Цвет линии направления воздуха - ВКЛ - светлая тема
        static constexpr QColor CLR_acLineLight_ON = QColor(250,0,0);
        /// Цвет линии направления воздуха - ВКЛ - темная тема
        static constexpr QColor CLR_acLineDark_ON = CLR_acLineLight_ON;
        /// Цвет линии направления воздуха - ВЫКЛ - светлая тема
        static constexpr QColor CLR_acLineLight_OFF = QColor(250,150,150);
        /// Цвет линии направления воздуха - ВЫКЛ - темная тема
        static constexpr QColor CLR_acLineDark_OFF = QColor(100,50,50);
    };
    /// @}

signals:
    /**
     * @brief Сигнал смены разрешения
     * @param res разрешение окна
     */
    void resolutionChanged(const QSize res);
    /// @brief Сигнал открытия окна ввода значений
    void openInputDialog();

private slots:
    /// @brief Уменьшение желаемой температуры
    void onMinusTargetTemp();
    /// @brief Увеличение желаемой температуры
    void onPlusTargetTemp();
    /// @brief ВКЛ/ВЫКЛ кондиционер
    void togglePower();
    /// @brief Обработчик смены значения слайдера (угла потока воздуха)
    void onSliderChanged(int value);
    /// @brief Смена единицы измерения температуры
    void changeTempUnit();
    /// @brief Смена единицы измерения давления
    void changePressureUnit();
    /// @brief Смена разрешения
    void changeResolution();
    /// @brief Смена темы по кнопке
    void changeTheme();
};

#endif // MAINSCENE_H
