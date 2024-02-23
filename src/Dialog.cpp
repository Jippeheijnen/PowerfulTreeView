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
    createFormGroupBox();

    bigEditor = new QTextEdit;
    bigEditor->setPlainText(tr("This widget takes up all the remaining space "
                               "in the top-level layout."));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(addButton, &QPushButton::clicked, this, &Dialog::addTreeItem);
    connect(removeButton, &QPushButton::clicked, this, &Dialog::removeTreeItem);
    connect(upButton, &QPushButton::clicked, this, &Dialog::moveTreeItemUp);
    connect(downButton, &QPushButton::clicked, this, &Dialog::moveTreeItemDown);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(gridGroupBox);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(buttonBox);
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
    setWindowTitle(tr("Basic Layouts"));
}

void Dialog::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    connect(exitAction, &QAction::triggered, this, &QDialog::accept);
}

void Dialog::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Horizontal layout"));
    QHBoxLayout *layout = new QHBoxLayout;

    addButton = new QPushButton(tr("Add"));
    removeButton = new QPushButton(tr("Remove"));
    upButton = new QPushButton(tr("Up"));
    downButton = new QPushButton(tr("Down"));

    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(upButton);
    layout->addWidget(downButton);

    horizontalGroupBox->setLayout(layout);
}

void Dialog::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Grid layout"));
    auto *layout = new QGridLayout;

    treeView = new TreeView;
    QStringList headers = {"Entry 1", "Entry 2", "Entry 3", "Entry 4"};
    treeModel = new TreeModel(headers, {});

    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    treeView->setModel(treeModel);

    layout->addWidget(treeView, 0, 2, 4, 1);

    gridGroupBox->setLayout(layout);
}

void Dialog::createFormGroupBox()
{
    formGroupBox = new QGroupBox(tr("Form layout"));
    QFormLayout *layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Line 1:")), new QLineEdit);
    layout->addRow(new QLabel(tr("Line 2, long text:")), new QComboBox);
    layout->addRow(new QLabel(tr("Line 3:")), new QSpinBox);
    formGroupBox->setLayout(layout);
}

void Dialog::addTreeItem() {
    qDebug() << "Add button pressed!";
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    treeView->model()->insertRow(index.row()+1, index.parent());

}

void Dialog::removeTreeItem() {
    qDebug() << "Remove button pressed!";
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    treeView->model()->removeRow(index.row(), index.parent());

}

void Dialog::moveTreeItemUp() {
    qDebug() << "Up button pressed!";

}

void Dialog::moveTreeItemDown() {
    qDebug() << "Down button pressed!";

}
