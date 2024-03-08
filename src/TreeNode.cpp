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

#include "TreeNode.hpp"

#include <QStringList>

TreeNode::TreeNode(const QList<QVariant> &data, TreeNode *parent)
        : m_nodeData(data), m_parentNode(parent)
{
    // only add content for non-root nodes.
    if (parent != nullptr) {
        contentItem = new ContentItem();
        contentItem->setName(this->data(0).toString());
        setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(contentItem)));
    }
}

TreeNode::~TreeNode()
{
    // remove the contentItem as well
    delete contentItem;
    qDeleteAll(m_childNodes);
}

void TreeNode::appendChild(TreeNode *node)
{
    m_childNodes.append(node);
}

void TreeNode::removeChild(int row)
{
    m_childNodes.removeAt(row);
}

TreeNode *TreeNode::child(int row) const
{
    return m_childNodes.value(row);
}

int TreeNode::childCount() const
{
    return m_childNodes.count();
}

int TreeNode::columnCount() const
{
    return m_nodeData.count();
}

bool TreeNode::setData(int column, const QVariant &value) {
    if (column == Qt::UserRole) {
        contentItem = static_cast<ContentItem*>(value.value<void*>());
        return true;
    }
    // change contentItem name when changing the node name
    if (column == 0) {
        contentItem->setName(value.toString());
    }

    if (column < 0 || column >= m_nodeData.size())
        return false;

    m_nodeData[column] = value;
    return true;
}

QVariant TreeNode::data(int column) const
{
    // handle the contentItem request
    if (column == Qt::UserRole) {
        return QVariant::fromValue(static_cast<void*>(contentItem));
    }
    return m_nodeData.value(column);
}

TreeNode *TreeNode::parentNode() const
{
    return m_parentNode;
}

int TreeNode::row() const
{
    if (m_parentNode)
        return m_parentNode->m_childNodes.indexOf(const_cast<TreeNode*>(this));

    return 0;
}

void TreeNode::insertChild(int pos, TreeNode *child)
{
    m_childNodes.insert(pos, child);
    child->m_parentNode = this;
}