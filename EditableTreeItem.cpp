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

#include "EditableTreeItem.hpp"

EditableTreeItem::EditableTreeItem(QVariantList data, EditableTreeItem *parent)
        : itemData(std::move(data)), m_parentItem(parent)
{}

EditableTreeItem *EditableTreeItem::child(int number)
{
    return (number >= 0 && number < childCount())
           ? m_childItems.at(number).get() : nullptr;
}

int EditableTreeItem::childCount() const
{
    return int(m_childItems.size());
}

int EditableTreeItem::row() const
{
    if (!m_parentItem)
        return 0;
    const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
                                 [this](const std::unique_ptr<EditableTreeItem> &EditableTreeItem) {
                                     return EditableTreeItem.get() == this;
                                 });

    if (it != m_parentItem->m_childItems.cend())
        return std::distance(m_parentItem->m_childItems.cbegin(), it);
    Q_ASSERT(false); // should not happen
    return -1;
}

int EditableTreeItem::columnCount() const
{
    return int(itemData.count());
}

QVariant EditableTreeItem::data(int column) const
{
    return itemData.value(column);
}

bool EditableTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > qsizetype(m_childItems.size()))
        return false;

    for (int row = 0; row < count; ++row) {
        QVariantList data(columns);
        m_childItems.insert(m_childItems.cbegin() + position,
                            std::make_unique<EditableTreeItem>(data, this));
    }

    return true;
}

bool EditableTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (auto &child : std::as_const(m_childItems))
        child->insertColumns(position, columns);

    return true;
}

EditableTreeItem *EditableTreeItem::parent()
{
    return m_parentItem;
}

bool EditableTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > qsizetype(m_childItems.size()))
        return false;

    for (int row = 0; row < count; ++row)
        m_childItems.erase(m_childItems.cbegin() + position);

    return true;
}

bool EditableTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for (auto &child : std::as_const(m_childItems))
        child->removeColumns(position, columns);

    return true;
}

bool EditableTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}