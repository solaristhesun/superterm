#include <QColor>
#include <QStringListModel>
#include <QSortFilterProxyModel>

#include "gmock/gmock.h"
#include "models/highlightingsmodel.h"
#include "models/highlighting.h"

TEST(HighlightingsModelsTests, rowCount_emptyModel_returnsZero)
{
    HighlightingsModel model;

    ASSERT_EQ(0, model.rowCount());

    Highlighting h;
    model.addHighlighting(h);

    ASSERT_EQ(1, model.rowCount());
}

TEST(HighlightingsModelsTests, rowCount_addOneHighlighting_returnsOne)
{
    HighlightingsModel model;
    Highlighting h;
    model.addHighlighting(h);

    ASSERT_EQ(1, model.rowCount());
}

TEST(HighlightingsModelsTests, addHighlighting)
{
    HighlightingsModel model;
    Highlighting h;
    h.pattern = "test";
    h.color = QColor(Qt::black);
    model.addHighlighting(h);

    QModelIndex index = model.index(0);

    ASSERT_EQ(QString("test"), model.data(index, Qt::DisplayRole));
}


TEST(HighlightingsModelsTests, proxyModelDebugging)
{
    HighlightingsModel model;
    Highlighting h1;
    h1.pattern = "zzz";
    model.addHighlighting(h1);

    Highlighting h2;
    h2.pattern = "aaa";
    model.addHighlighting(h2);

    ASSERT_EQ("zzz", model.data(model.index(0), Qt::DisplayRole));
    ASSERT_EQ("aaa", model.data(model.index(1), Qt::DisplayRole));

    QSortFilterProxyModel proxyModel;
    proxyModel.setSourceModel(&model);
    proxyModel.setSortRole(Qt::DisplayRole);
    proxyModel.setDynamicSortFilter(true);
    proxyModel.sort(0);

    ASSERT_EQ("aaa", proxyModel.data(proxyModel.index(0, 0), Qt::DisplayRole));
    ASSERT_EQ("zzz", proxyModel.data(proxyModel.index(1, 0), Qt::DisplayRole));

    model.remove(proxyModel.mapToSource(proxyModel.index(0,0)));
    ASSERT_EQ(1, model.rowCount());
    ASSERT_EQ(1, proxyModel.rowCount());
}


TEST(HighlightingsModelsTests, proxyModelDebugging2)
{
    QStringList stringList;
    stringList << "zzz" << "aaa";

    QStringListModel model;
    model.setStringList(stringList);

    ASSERT_EQ("zzz", model.data(model.index(0), Qt::DisplayRole));
    ASSERT_EQ("aaa", model.data(model.index(1), Qt::DisplayRole));

    QSortFilterProxyModel proxyModel;
    proxyModel.setSourceModel(&model);
    proxyModel.setSortRole(Qt::DisplayRole);
    proxyModel.setDynamicSortFilter(true);
    proxyModel.sort(0);

    ASSERT_EQ("aaa", proxyModel.data(proxyModel.index(0, 0), Qt::DisplayRole));
    ASSERT_EQ("zzz", proxyModel.data(proxyModel.index(1, 0), Qt::DisplayRole));

    model.removeRows(proxyModel.mapToSource(proxyModel.index(0,0)).row(), 1);
    //model.removeRows(0, 1);
    ASSERT_EQ(1, model.rowCount());
    ASSERT_EQ(1, proxyModel.rowCount());
}

// EOF <stefan@scheler.com>
