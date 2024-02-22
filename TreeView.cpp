/*
Created by Jippe Heijnen on 13-2-24.

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

#include "TreeView.hpp"
#include "TreeModel.hpp"
#include <QHeaderView>

TreeView::TreeView(TreeModel *model)
{
    setModel(model);
    TreeView();
}

TreeView::TreeView()
{
    connect(this, &QTreeView::pressed, this, &QTreeView::expandAll);
    setDragEnabled(true);
    setAcceptDrops(true);
    resizeColumnToContents(0);
    resize(400, 500);
    setSelectionMode(QAbstractItemView::SingleSelection);
    expandAll();
    connect(this, &QTreeView::pressed, this, &QTreeView::expandAll);
    header()->setVisible(false);
    setIndentation(5);
}

void TreeView::setModel(TreeModel *model)
{
    //Kick what's already selected (http://doc.qt.io/qt-4.8/qabstractitemview.html#setModel)
    QItemSelectionModel *m = selectionModel();
    delete m;
    QTreeView::setModel(model);
}