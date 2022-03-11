#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include <QAbstractItemDelegate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTableModel(QAbstractTableModel* iModel);
    void setTableItemDelegate(QAbstractItemDelegate* iDelegate);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
