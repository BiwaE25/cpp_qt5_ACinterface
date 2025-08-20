#include <QApplication>
#include <QGraphicsView>
#include "mainscene.h"
#include "inputdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Система управление кондиционером");
    /// Класс MainScene инициализируется, как QGraphicsScene
    MainScene *scene = new MainScene();
    QSize res = scene->prefs->getResolution();
    /// Создаётся QGraphicsView для показа MainScene
    QGraphicsView *view = new QGraphicsView(scene);

    view->setSceneRect(0,0,res.width(),res.height());
    view->setFixedSize(res);

    /// Отключение горизонтальных скроллбаров
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    /// Отключение вертикальных скроллбаров
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    /// @brief Сохранение параметров при выходе
    QObject::connect(&app, &QApplication::aboutToQuit, [scene]() {
        scene->savePrefs();
    });

    /// @brief Открытие окна для ввода параметров, и их сохранение
    QObject::connect(scene,&MainScene::openInputDialog, [&]() {
        InputDialog dialog;
        dialog.setValues(scene->prefs);
        if (dialog.exec() == QDialog::Accepted) {
            scene->prefs->setTempVal(dialog.getTemp());
            scene->prefs->setHumidityVal(dialog.getHumidity());
            scene->prefs->setPressureVal(dialog.getPressure());
            scene->updateValues();
            scene->updatePos();
        }
    });

    /// @brief Изменение разрешения
    QObject::connect(scene,&MainScene::resolutionChanged, [&](const QSize &res) {
        view->setSceneRect(0, 0, res.width(), res.height());
        view->setFixedSize(res);
    });

    return app.exec();    
}
