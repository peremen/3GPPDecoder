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

#ifndef DISSECTORMODEL_H
#define DISSECTORMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QPair>
#include <QString>

class DissectorModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DissectorModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addData(QString name, QString dissector);

signals:

public slots:

private:
    QList<QPair<QString, QString> > dissector;

};

#endif // DISSECTORMODEL_H
