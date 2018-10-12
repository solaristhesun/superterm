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

#include <QDebug>
#include <QColorDialog>
#include <QKeyEvent>
#include <QDataStream>
#include <QSortFilterProxyModel>

#include "ui_highlightingsframe.h"
#include "ui/highlightingsframe.h"
#include "models/highlightingsmodel.h"

HighlightingsFrame::HighlightingsFrame(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::HighlightingsFrame)
    , proxyModel_(new QSortFilterProxyModel(this))
    , model_(nullptr)
{
    ui_->setupUi(this);

    proxyModel_->setSortRole(Qt::DisplayRole);
    proxyModel_->setDynamicSortFilter(true);
    proxyModel_->sort(0);

    QFrame::hide();

    highlighting_.color = QColor(Qt::red);
    refreshColorButton();
}

HighlightingsFrame::~HighlightingsFrame()
{
    delete proxyModel_;
    delete ui_;
}

void HighlightingsFrame::addHighlighting()
{
    model_->addHighlighting(highlighting_);

    ui_->patternEdit->setText("");
    ui_->patternEdit->setFocus();
    ui_->btnDeleteAll->setEnabled(true);
}

void HighlightingsFrame::showEvent(QShowEvent* event)
{
    ui_->patternEdit->setText("");
    ui_->patternEdit->setFocus();

    if (model_->rowCount() == 0)
    {
        ui_->btnDeleteAll->setEnabled(false);
    }
    else
    {
        ui_->btnDeleteAll->setEnabled(true);
    }

    QFrame::showEvent(event);
}

void HighlightingsFrame::onTextEdited(const QString& text)
{
    highlighting_.pattern = text;
    ui_->btnAdd->setEnabled(!text.isEmpty());
}

void HighlightingsFrame::onSelectionChanged()
{
    ui_->btnDelete->setEnabled(ui_->patternView->currentIndex().isValid());
}

void HighlightingsFrame::changeColor()
{
    QColor color = QColorDialog::getColor(QColor("red"), this);

    if (color.isValid())
    {
        highlighting_.color = color;
        refreshColorButton();
    }
}

void HighlightingsFrame::refreshColorButton()
{
    ui_->btnColor->setStyleSheet(QString("background-color: %1; border: 1px solid %2;")
        .arg(highlighting_.color.name())
        .arg(highlighting_.color.darker(150).name()));
}

void HighlightingsFrame::deleteHighlighting()
{
    model_->remove(proxyModel_->mapToSource(ui_->patternView->currentIndex()));
}

void HighlightingsFrame::deleteAll()
{
    model_->removeAll();
    ui_->btnDeleteAll->setEnabled(false);
}

void HighlightingsFrame::selectionChanged(const QItemSelection &selection, const QItemSelection &)
{
    if (selection.empty())
    {
        ui_->btnDeleteAll->setEnabled(false);
        ui_->btnDelete->setEnabled(false);
    }
    else
    {
        ui_->btnDeleteAll->setEnabled(true);
        ui_->btnDelete->setEnabled(true);
    }
}

void HighlightingsFrame::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QFrame::hide();
    }

    QFrame::keyPressEvent(event);
}

void HighlightingsFrame::setModel(HighlightingsModel* model)
{
    model_ = model;
    proxyModel_->setSourceModel(model);
    ui_->patternView->setModel(proxyModel_);

    QItemSelectionModel *selectionModel= ui_->patternView->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChanged(const QItemSelection &, const QItemSelection &)));
}

QDataStream& operator<<(QDataStream& out, const Highlighting& v)
{
    out << v.pattern << v.color;
    return out;
}

QDataStream& operator>>(QDataStream& in, Highlighting& v)
{
    in >> v.pattern;
    in >> v.color;
    return in;
}

// EOF <stefan@scheler.com>
