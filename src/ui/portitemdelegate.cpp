/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QDebug>
#include <QPainter>

#include "serial/serialportinfo.h"
#include "ui/portitemdelegate.h"
#include "ui/portitemview.h"

PortItemDelegate::PortItemDelegate()
    : view_(new PortItemView(nullptr))
{
    // currently emtpy
}


void PortItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    SerialPortInfo info = qvariant_cast<SerialPortInfo>(index.data(Qt::UserRole));

    QPalette pal;

    if (option.state & QStyle::State_Selected)
    {
        pal.setColor(QPalette::Background, option.palette.highlight().color());
    }
    else
    {
        pal.setColor(QPalette::Background, QColor("white"));
    }

    if (index.row() == 0)
    {
        QItemDelegate::paint(painter, option, index);
        return;
    }

    view_->setPortName(info.shortName());
    view_->setDescription("[" + info.description() + "]");
    view_->setBusyFlag(info.isBusy());
    view_->setPalette(pal);

    painter->save();
    painter->translate(option.rect.topLeft());
    view_->render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}

QSize PortItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return view_->size();
}

// EOF <stefan@scheler.com>
