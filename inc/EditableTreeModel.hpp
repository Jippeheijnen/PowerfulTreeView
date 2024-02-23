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

#ifndef TREEVIEWTEST_EDITABLETREEMODEL_HPP
#define TREEVIEWTEST_EDITABLETREEMODEL_HPP

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "EditableTreeItem.hpp"

class TreeItem;

//! [0]
class EditableTreeModel : public QAbstractItemModel
{
Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(EditableTreeModel)

    EditableTreeModel(const QStringList &headers, const QString &data,
              QObject *parent = nullptr);
    ~EditableTreeModel() override;
//! [0] //! [1]

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
//! [1]

//! [2]
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = {}) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = {}) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = {}) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = {}) override;

private:
    void setupModelData(const QList<QStringView> &lines);
    EditableTreeItem *getItem(const QModelIndex &index) const;

    std::unique_ptr<EditableTreeItem> rootItem;
};


#endif //TREEVIEWTEST_EDITABLETREEMODEL_HPP
