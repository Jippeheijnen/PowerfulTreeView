#include <iostream>
#include <QApplication>
#include <QStandardItemModel>
#include <QStandardItem>
#include "TreeView.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    TreeModel model({"1", "2", "3"});

    TreeView treeView;
    treeView.setDragEnabled(true);
    treeView.setAcceptDrops(true);
    treeView.setDragDropMode(QAbstractItemView::InternalMove);
    treeView.setModel(&model);
    model.insertRows(0, 1, treeView.currentIndex());


    // Show the tree view
    treeView.show();

    return app.exec();

}
