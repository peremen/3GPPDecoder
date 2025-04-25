/* This file is part of 3GPP Decoder project.
 * Copyright (C) 2015-2025 Shinjo Park
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

#include "dissectormodel.h"

DissectorModel::DissectorModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int DissectorModel::rowCount(const QModelIndex &) const {
    return dissector.count();
}

QVariant DissectorModel::headerData(int section, Qt::Orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    return QString("%1").arg(section);
}

QVariant DissectorModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= dissector.count()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return dissector.at(index.row()).first;
    } else if (role == Qt::UserRole) {
        return dissector.at(index.row()).second;
    } else {
        return QVariant();
    }
}

void DissectorModel::addData(QString name, QString dissector_t) {
    dissector.append(QPair<QString, QString>(name, dissector_t));
}
