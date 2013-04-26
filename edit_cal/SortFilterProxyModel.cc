#include <QDebug>
#include <iostream>
#include "SortFilterProxyModel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

/* -------------------------------------------------------------------- */

void SortFilterProxyModel::setFilter(int column, const QString &pattern)
{
    qDebug() << __PRETTY_FUNCTION__ << "column:" << column  << "pattern:" << pattern;

    if (pattern.isEmpty())
        m_columnPatterns.remove(column);
    else
        m_columnPatterns[column] = QRegExp(pattern, Qt::CaseInsensitive);

    invalidateFilter();
}

/* -------------------------------------------------------------------- */

QString SortFilterProxyModel::filterAt(int column)
{
    if (m_columnPatterns.contains(column))
        return m_columnPatterns[column].pattern();
    else
        return "";
}

/* -------------------------------------------------------------------- */

void SortFilterProxyModel::clearFilters()
{
    if(m_columnPatterns.isEmpty())
        return;

    qDebug() << __PRETTY_FUNCTION__;
    m_columnPatterns.clear();
    invalidateFilter();
}

/* -------------------------------------------------------------------- */

void SortFilterProxyModel::refreshFilters()
{
    invalidateFilter();
}

/* -------------------------------------------------------------------- */

bool SortFilterProxyModel::filterAcceptsRow(int sourceRow, 
  const QModelIndex &sourceParent) const
{
    if(m_columnPatterns.isEmpty())
        return true;

    bool ret = false;

    for(QMap<int, QRegExp>::const_iterator iter = m_columnPatterns.constBegin();
        iter != m_columnPatterns.constEnd(); ++iter)
    {
        QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
        ret = index.data().toString().contains(iter.value());

        if(!ret)
            return ret;
    }

    return ret;
}
