/*
Created by Jippe Heijnen on 22-2-24.

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Dialog.hpp"
#include "TreeView.hpp"
#include "TreeNode.hpp"
#include <QtWidgets>

Dialog::Dialog()
{
    createMenu();
    createHorizontalGroupBox();
    createGridGroupBox();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(addButton, &QPushButton::clicked, this, &Dialog::addTreeItem);
    connect(removeButton, &QPushButton::clicked, this, &Dialog::removeTreeItem);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(gridGroupBox);
    QWidget *scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {width: 20px;}");

    scrollArea->setWidget(scrollAreaContent);

    QVBoxLayout *scrollLayout = new QVBoxLayout;
    scrollLayout->setContentsMargins(0,0,0,0);
    scrollLayout->addWidget(scrollArea);

    setLayout(scrollLayout);
    setWindowTitle(tr("Powerful TreeView"));
}

void Dialog::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    saveAction = fileMenu->addAction(tr("&Save"));
    loadAction = fileMenu->addAction(tr("&Load"));
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    connect(exitAction, &QAction::triggered, this, &QDialog::accept);
    connect(saveAction, &QAction::triggered, this, &Dialog::save);
    connect(loadAction, &QAction::triggered, this, &Dialog::load);
}

void Dialog::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Adding / removing rows"));
    QHBoxLayout *layout = new QHBoxLayout;

    addButton = new QPushButton(tr("Add"));
    removeButton = new QPushButton(tr("Remove"));

    layout->addWidget(addButton);
    layout->addWidget(removeButton);

    horizontalGroupBox->setLayout(layout);
}

void Dialog::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Tree View"));
    auto *layout = new QGridLayout;

    treeView = new TreeView;
    treeModel = new TreeModel({}, {});

    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    treeView->setModel(treeModel);

    layout->addWidget(treeView, 0, 2, 4, 1);

    gridGroupBox->setLayout(layout);
}

void Dialog::addTreeItem() {
    qDebug() << "Add button pressed!";
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    auto m = treeModel;
    m->insertNode(index.row()+1, index.parent());

}

void Dialog::removeTreeItem() {
    qDebug() << "Remove button pressed!";
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    auto m = treeModel;
    m->removeNode(index.row(), index.parent());

}

void Dialog::save() {
    qDebug() << "Save pressed";

    QJsonArray output;

    // implement recursive save function
    std::function<QJsonObject(TreeNode*)>  saveNodeRecursively;
    saveNodeRecursively = [&saveNodeRecursively](TreeNode *node) ->QJsonObject {
        QJsonArray output = {};
        int childCounter = 0;
        auto child = node->child(childCounter);
        while (child) {
            output.append(saveNodeRecursively(child));
            childCounter++;
            child = node->child(childCounter);
        }

        QJsonObject entryJsonObject;
        entryJsonObject[node->data(0).toString()] = output;
        return entryJsonObject;
    };

    auto rootNode = treeModel->getRootNode();
    auto jsonOutput = saveNodeRecursively(rootNode);

    // Save the JSON data to a file
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "./tree.json",
                                                    tr("Tree structure (*.json)"));

    QFile outputFile(filePath);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream outStream(&outputFile);
        outStream << QJsonDocument(jsonOutput).toJson();
        outputFile.close();
    }
}

void Dialog::load() {
    qDebug() << "Load pressed";

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                            "./",
                                            tr("Tree structure (*.json)"));

    // Read JSON data from the file
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file for reading:" << inputFile.errorString();
        return;
    }

    QByteArray jsonData = inputFile.readAll();
    inputFile.close();

    // Parse JSON data
    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    auto * rootNode = new TreeNode({"root"}, 0);
    treeModel->setRootNode(rootNode);
    qDebug() << jsonDoc.object()["Root"];
    QJsonArray testArray = jsonDoc.object()["Root"].toArray();
    for (auto item : testArray) {
        qDebug() << item;
    }
    for (auto node : jsonDoc.array()) {
        qDebug() << node.toObject();
    }

}