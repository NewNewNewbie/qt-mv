#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::setTableModel(
        QAbstractTableModel* iModel
        )
{
    ui->tableView->setModel(iModel);
}

void
MainWindow::setTableItemDelegate(
        QAbstractItemDelegate* iDelegate
        )
{
    ui->tableView->setItemDelegate(iDelegate);
}



