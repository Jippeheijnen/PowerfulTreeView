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

#ifndef TREEVIEWTEST_EDITABLETREEITEM_HPP
#define TREEVIEWTEST_EDITABLETREEITEM_HPP


#include <QVariant>
#include <QList>

class EditableTreeItem
{
public:
    explicit EditableTreeItem(QVariantList data, EditableTreeItem *parent = nullptr);

    EditableTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    EditableTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int row() const;
    bool setData(int column, const QVariant &value);

private:
    std::vector<std::unique_ptr<EditableTreeItem>> m_childItems;
    QVariantList itemData;
    EditableTreeItem *m_parentItem;
};


#endif //TREEVIEWTEST_EDITABLETREEITEM_HPP
