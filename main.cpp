#include <iostream>
#include <QApplication>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPushButton>
#include <QWindow>
#include "TreeView.hpp"
#include "Dialog.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);


    Dialog * d = new Dialog;

    // Show the d
    d->show();

    return app.exec();

}
