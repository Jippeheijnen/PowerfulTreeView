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

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QObject>
#include <QTreeView>
#include "TreeModel.hpp"

class TreeView : public QTreeView
{
public:
    TreeView(TreeModel *model);
    TreeView();
    void setModel(TreeModel *model);
};

#endif // TREEVIEW_H