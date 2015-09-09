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
