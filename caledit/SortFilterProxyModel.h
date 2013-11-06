#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

/**
 * @class SortFilterProxyModel
 * This derived class allows for multiple column filters.
 */
class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent = 0);
    void setFilter(int column, const QString &pattern);
    QString filterAt(int column);
    void clearFilters();
    void refreshFilters();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QMap<int, QRegExp> m_columnPatterns;
};

#endif // SORTFILTERPROXYMODEL_H
