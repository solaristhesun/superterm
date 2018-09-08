#include "portitemdelegate.h"

#include <QApplication>

PortItemDelegate::PortItemDelegate()
{

}


void PortItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if 0
    //SearchResult sr = qvariant_cast<SearchResult>(index.data());

    QPalette pal;

    if (option.state & QStyle::State_Selected)
    {
        pal.setColor(QPalette::Background, QColor("#9dc1ca"));
    }
    else
    {
        pal.setColor(QPalette::Background, QColor("white"));
    }

    QPixmap pixmap("adimages:" + sr.AdId + ".jpg");
    searchResultView_->setPixmap(pixmap);
    searchResultView_->setTitle(sr.AdTitle);
    searchResultView_->setDescription(sr.AdDescription);
    searchResultView_->setDate(sr.AdStart.toString("dd.MM.yyyy"));
    searchResultView_->setTagLabel(sr.AdStart);
    searchResultView_->resize(option.rect.size());
    searchResultView_->setPalette(pal);

    QString priceText = QString::number((double)sr.AdPrice / 100.0) + " €";
    searchResultView_->setPriceLabel(priceText);
    searchResultView_->setPlaceLabel(sr.AdDistance);

    painter->save();
    painter->translate(option.rect.topLeft());
    searchResultView_->render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter->restore();

    painter->setPen(QColor("#8fa7cb"));

    if (sr.bSeen)
    {
        painter->fillRect(option.rect, QBrush(QColor(255, 255, 255, 160)));
    }
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
#endif

    QStyleOptionViewItemV4 myOption = option;
    QString text = index.data().toString();

    myOption.text = text;

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
/*
QSize PortItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(30, 20+1);
}*/
