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

#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include <QVariant>
#include "structure.h"

class TreeNode
{
public:
    explicit TreeNode(const QList<QVariant> &data, TreeNode *parentNode);
    ~TreeNode();

    void appendChild(TreeNode *child);
    void removeChild(int row);

    TreeNode *child(int row) const;
    int childCount() const;
    int columnCount() const;
    bool setData(int column, const QVariant &value);
    QVariant data(int column) const;
    int row() const;
    TreeNode *parentNode() const;
    void insertChild(int pos, TreeNode *child);
    TreeNode *m_parentNode;
    QList<TreeNode*> m_childNodes;
private:
    QList<QVariant> m_nodeData;
    ContentItem *contentItem;
};

#endif // TREENODE_H
