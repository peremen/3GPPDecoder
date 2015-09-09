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
    /*
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    */

    void addData(QString name, QString dissector);

signals:

public slots:

private:
    QList<QPair<QString, QString> > dissector;

};

#endif // DISSECTORMODEL_H
