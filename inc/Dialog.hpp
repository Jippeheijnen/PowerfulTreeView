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

#ifndef TREEVIEWTEST_DIALOG_HPP
#define TREEVIEWTEST_DIALOG_HPP


#include <QDialog>
#include <QMenuBar>
#include <QGroupBox>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include "TreeView.hpp"
#include "EditableTreeModel.hpp"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();

private:
    void createMenu();
    void createHorizontalGroupBox();
    void createGridGroupBox();

private slots:
    void addTreeItem();
    void removeTreeItem();
    void save();
    void load();


private:
    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox;
    QGroupBox *gridGroupBox;
    QPushButton * addButton;
    QPushButton * removeButton;
    QDialogButtonBox *buttonBox;

    TreeView *treeView;
    TreeModel *treeModel;

    QMenu *fileMenu;
    QAction *saveAction;
    QAction *loadAction;
    QAction *exitAction;
};


#endif //TREEVIEWTEST_DIALOG_HPP
