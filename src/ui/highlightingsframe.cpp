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
    ui_->btnColor->setStyleSheet(QString("background-color: %1;").arg(highlighting_.color.name()));
}

void HighlightingsFrame::deleteHighlighting()
{
    //model_->remove(proxyModel_->mapToSource(ui_->patternView->currentIndex()));
    model_->remove(ui_->patternView->currentIndex());
    ui_->btnDelete->setEnabled(false);
}

void HighlightingsFrame::deleteAll()
{
    model_->removeAll();
    ui_->btnDeleteAll->setEnabled(false);
}

void HighlightingsFrame::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        hide();
    }

    QFrame::keyPressEvent(event);
}

void HighlightingsFrame::setModel(HighlightingsModel* model)
{
    model_ = model;
    //proxyModel_->setSourceModel(model);

    //proxyModel_->setSortRole(Qt::DisplayRole);
    //proxyModel_->setDynamicSortFilter(true);
    //proxyModel_->sort(0);
    ui_->patternView->setModel(model_);
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
