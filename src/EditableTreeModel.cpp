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

#include "EditableTreeModel.hpp"
#include "EditableTreeItem.hpp"

using namespace Qt::StringLiterals;

EditableTreeModel::EditableTreeModel(const QStringList &headers, const QString &data, QObject *parent)
        : QAbstractItemModel(parent)
{
    QVariantList rootData;
    for (const QString &header : headers)
        rootData << header;

    rootItem = std::make_unique<EditableTreeItem>(rootData);
    setupModelData(QStringView{data}.split(u'\n'));
}

EditableTreeModel::~EditableTreeModel() = default;

int EditableTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant EditableTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return {};

    EditableTreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags EditableTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

EditableTreeItem *EditableTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (auto *item = static_cast<EditableTreeItem*>(index.internalPointer()))
            return item;
    }
    return rootItem.get();
}

QVariant EditableTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    return (orientation == Qt::Horizontal && role == Qt::DisplayRole)
           ? rootItem->data(section) : QVariant{};
}

QModelIndex EditableTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return {};

    EditableTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return {};

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

bool EditableTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool EditableTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    EditableTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex EditableTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    EditableTreeItem *childItem = getItem(index);
    EditableTreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    return (parentItem != rootItem.get() && parentItem != nullptr)
           ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

bool EditableTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool EditableTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    EditableTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int EditableTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const EditableTreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool EditableTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    EditableTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

bool EditableTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value);

    if (result)
            emit headerDataChanged(orientation, section, section);

    return result;
}

void EditableTreeModel::setupModelData(const QList<QStringView> &lines)
{
    struct ParentIndentation
    {
        EditableTreeItem *parent;
        qsizetype indentation;
    };

    QList<ParentIndentation> state{{rootItem.get(), 0}};

    for (const auto &line : lines) {
        qsizetype position = 0;
        for ( ; position < line.length() && line.at(position).isSpace(); ++position) {
        }

        const QStringView lineData = line.sliced(position).trimmed();
        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const auto columnStrings = lineData.split(u'\t', Qt::SkipEmptyParts);
            QVariantList columnData;
            columnData.reserve(columnStrings.count());
            for (const auto &columnString : columnStrings)
                columnData << columnString.toString();

            if (position > state.constLast().indentation) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.
                auto *lastParent = state.constLast().parent;
                if (lastParent->childCount() > 0)
                    state.append({lastParent->child(lastParent->childCount() - 1), position});
            } else {
                while (position < state.constLast().indentation && !state.isEmpty())
                    state.removeLast();
            }

            // Append a new item to the current parent's list of children.
            auto *parent = state.constLast().parent;
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData.at(column));
        }
    }
}