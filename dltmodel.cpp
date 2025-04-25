/* This file is part of 3GPP Decoder project.
 * Copyright (C) 2025 Shinjo Park
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

#include "dltmodel.h"

DltModel::DltModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int DltModel::rowCount(const QModelIndex &) const {
    return 16;
}

QVariant DltModel::headerData(int section, Qt::Orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    return QString("%1").arg(section);
}

QVariant DltModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= 16) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QString("User %1 (DLT=%2)").arg(index.row()).arg(index.row() + 147);
    } else if (role == Qt::UserRole) {
        return index.row();
    } else {
        return QVariant();
    }
}
