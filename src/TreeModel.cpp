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
#include "TreeModel.hpp"

#include <QCoreApplication>
#include <QStringList>
#include <QMimeData>
#include <QIODevice>
#include <QDataStream>

TreeModel::TreeModel(QStringList commodities, QObject *parent) : QAbstractItemModel(parent)
{

    QStringList data = commodities;

    QList<QVariant> headerData;
    headerData << "Scene entries";
    m_rootNode = new TreeNode(headerData, 0);
    setupModelData(data, m_rootNode);
}
TreeModel::~TreeModel()
{
    delete m_rootNode;
}

static const char s_treeNodeMimeType[] = "application/x-treenode";

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    auto node = nodeForIndex(index);
    node->setData(index.column(), value);
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole &&
        role != Qt::EditRole &&
        role != Qt::UserRole)
        return QVariant();

    TreeNode *node = nodeForIndex(index);

    // handle contentItem requests
    if (role == Qt::UserRole) {
        return node->data(Qt::UserRole);
    }

    return node->data(index.column());
}
QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootNode->data(section);

    return QVariant();
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentNode = nodeForIndex(parent);

    TreeNode *childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    TreeNode *childNode = nodeForIndex(index);
    if (childNode == m_rootNode)
        return QModelIndex();

    TreeNode *parentNode = childNode->parentNode();
    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}
int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    TreeNode *parentNode = nodeForIndex(parent);
    return parentNode->childCount();
}
int TreeModel::columnCount(const QModelIndex &parent) const
{
    return nodeForIndex(parent)->columnCount();
}
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return QAbstractItemModel::flags(index)| Qt::ItemIsEditable |Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}
Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
Qt::DropActions TreeModel::supportedDragActions() const
{
    return Qt::MoveAction;
}
//returns the mime type
QStringList TreeModel::mimeTypes() const
{
    return QStringList() << s_treeNodeMimeType;
}
//receives a list of model indexes list
QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData;
    QByteArray data; //a kind of RAW format for datas

    //QDataStream is independant on the OS or proc architecture
    //serialization of C++'s basic data types, like char, short, int, char *, etc.
    //Serialization of more complex data is accomplished
    //by breaking up the data into primitive units.
    QDataStream stream(&data, QIODevice::WriteOnly);
    QList<TreeNode *> nodes;

    //
    foreach (const QModelIndex &index, indexes) {
        TreeNode *node = nodeForIndex(index);
        if (!nodes.contains(node))
            nodes << node;
    }
    stream << QCoreApplication::applicationPid();
    int count = nodes.count();
    stream << count;
    foreach (TreeNode *node, nodes) {
        stream << reinterpret_cast<qlonglong>(node);
    }
    mimeData->setData(s_treeNodeMimeType, data);
    return mimeData;
}
bool TreeModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_ASSERT(action == Qt::MoveAction);
    Q_UNUSED(column);
    //test if the data type is the good one
    if (!mimeData->hasFormat(s_treeNodeMimeType)) {
        return false;
    }
    QByteArray data = mimeData->data(s_treeNodeMimeType);
    QDataStream stream(&data, QIODevice::ReadOnly);
    qint64 senderPid;
    stream >> senderPid;
    if (senderPid != QCoreApplication::applicationPid()) {
        // Let's not cast pointers that come from another process...
        return false;
    }
    TreeNode *parentNode = nodeForIndex(parent);
    Q_ASSERT(parentNode);
    int count;
    stream >> count;
    if (row == -1) {
        // valid index means: drop onto item. I chose that this should insert
        // a child item, because this is the only way to create the first child of an item...
        // This explains why Qt calls it parent: unless you just support replacing, this
        // is really the future parent of the dropped items.
        if (parent.isValid())
            row = 0;
        else
            // invalid index means: append at bottom, after last toplevel
            row = rowCount(parent);
    }
    for (int i = 0; i < count; ++i) {
        // Decode data from the QMimeData
        qlonglong nodePtr;
        stream >> nodePtr;
        TreeNode *node = reinterpret_cast<TreeNode *>(nodePtr);

        // Adjust destination row for the case of moving an item
        // within the same parent, to a position further down.
        // Its own removal will reduce the final row number by one.
        if (node->row() < row && parentNode == node->parentNode())
            --row;

        // Remove from old position
        removeNode(node);

        // Insert at new position
        //qDebug() << "Inserting into" << parent << row;
        beginInsertRows(parent, row, row);
        parentNode->insertChild(row, node);
        endInsertRows();
        ++row;
    }
    return true;
}

bool TreeModel::insertNode(int row, const QModelIndex &parent) {

    TreeNode *parentItem = nodeForIndex(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, row, row);
    parentItem->insertChild(row, new TreeNode({"New Entry"}, parentItem));
    endInsertRows();

    return true;

}

bool TreeModel::removeNode(int row, const QModelIndex &parent) {

    TreeNode *parentItem = nodeForIndex(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, row, row);
    parentItem->removeChild(row);
    endRemoveRows();

    return true;
}

TreeNode * TreeModel::getRootNode() const {
    return this->m_rootNode;
}

void TreeModel::setRootNode(TreeNode * rootNode) {
    this->m_rootNode = rootNode;
}

//returns a pointer to the "index"
TreeNode * TreeModel::nodeForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootNode;
    else
        return static_cast<TreeNode*>(index.internalPointer());
}
void TreeModel::removeNode(TreeNode *node)
{
    const int row = node->row();
    QModelIndex idx = createIndex(row, 0, node);
    beginRemoveRows(idx.parent(), row, row);
    node->parentNode()->removeChild(row);
    endRemoveRows();
}
void TreeModel::setupModelData(const QStringList &lines, TreeNode *parent)
{
    QList<TreeNode*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", Qt::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new node to the current parent's list of children.
            parents.last()->appendChild(new TreeNode(columnData, parents.last()));
        }

        ++number;
    }
}