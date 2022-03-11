#include "mainwindow.h"
#include "testmodel.h"
#include "itemdelegate.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    TestModel* lTableModel = new TestModel(&w);

    {
        auto lObject = std::make_shared<Object>("Объект 1");

        lObject->emplace("Ключ 1",std::make_unique<Boolean>(true));
        lObject->emplace("Ключ 2",std::make_unique<String>("string"));

        lTableModel->addObject(std::move(lObject));
    }
    {
        auto lObject = std::make_shared<Object>("Объект 2");

        lObject->emplace("Ключ 2",std::make_unique<Double>(0.5));
        lObject->emplace("Ключ 3",std::make_unique<String>("hello"));
        lObject->emplace("Ключ 4",std::make_unique<Boolean>());

        lTableModel->addObject(std::move(lObject));
    }

    w.setTableModel(lTableModel);

    ItemDelegate* lTableItemDelegate = new ItemDelegate(&w);

    w.setTableItemDelegate(lTableItemDelegate);

    w.show();

    return a.exec();
}
