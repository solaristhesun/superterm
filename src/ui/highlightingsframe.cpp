#include <QDebug>
#include <QColorDialog>
#include <QKeyEvent>
#include <QDataStream>

#include "ui_highlightingsframe.h"
#include "ui/highlightingsframe.h"

CHighlightingsFrame::CHighlightingsFrame(QWidget* parent)
    : QFrame(parent)
    , m_ui(new Ui::CHighlightingsFrame)
    , m_color(QColor(Qt::red))
{
    m_ui->setupUi(this);
    hide();
    refreshColorButton();
}

CHighlightingsFrame::~CHighlightingsFrame()
{
    delete m_ui;
}

void CHighlightingsFrame::addHighlighting()
{
    const QString str = m_ui->filterEdit->text();

    if (!str.isEmpty())
    {
        QPixmap pixmap(10, 10);
        pixmap.fill(m_color);
        QIcon            icon(pixmap);
        QListWidgetItem* item = new QListWidgetItem(icon, str);
        item->setData(Qt::UserRole, QVariant(m_color));
        m_ui->filterList->addItem(item);
        m_ui->filterEdit->setText("");
        m_ui->filterEdit->setFocus();
        m_ui->btnDeleteAll->setEnabled(true);
    }

    emit highlightingChanged();
}

void CHighlightingsFrame::addHighlighting(QListWidgetItem* item)
{
    m_ui->filterList->addItem(item);
}

void CHighlightingsFrame::showEvent(QShowEvent* event)
{
    m_ui->filterEdit->setText("");
    m_ui->filterEdit->setFocus();

    if (m_ui->filterList->count() == 0)
    {
        m_ui->btnDeleteAll->setEnabled(false);
    }
    else
    {
        m_ui->btnDeleteAll->setEnabled(true);
    }

    QFrame::showEvent(event);
}

void CHighlightingsFrame::onTextEdited(const QString& text)
{
    m_ui->btnAdd->setEnabled(!text.isEmpty());
}

void CHighlightingsFrame::onSelectionChanged()
{
    QList<QListWidgetItem*> items = m_ui->filterList->selectedItems();
    qDebug() << "COUNT " << items.count();
    m_ui->btnDelete->setEnabled(!items.isEmpty());
}

void CHighlightingsFrame::changeColor()
{
    QColor color = QColorDialog::getColor(QColor("red"), this);
    if (color.isValid())
    {
        m_color = color;
        refreshColorButton();
    }
}

void CHighlightingsFrame::refreshColorButton()
{
    m_ui->btnColor->setStyleSheet(QString("background-color: %1;").arg(m_color.name()));
}

void CHighlightingsFrame::deleteHighlighting()
{
    QListWidgetItem* item = m_ui->filterList->currentItem();
    delete item;

    if (m_ui->filterList->count() == 0)
    {
        m_ui->btnDeleteAll->setEnabled(false);
    }

    emit highlightingChanged();
}

void CHighlightingsFrame::deleteAll()
{
    m_ui->filterList->clear();
    m_ui->btnDeleteAll->setEnabled(false);

    emit highlightingChanged();
}

void CHighlightingsFrame::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        hide();
    }

    QFrame::keyPressEvent(event);
}

void CHighlightingsFrame::clear()
{
    m_ui->filterList->clear();
}

QList<Highlighting> CHighlightingsFrame::getItems()
{
    QList<QListWidgetItem*> items = m_ui->filterList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QList<Highlighting>     list;

    for (QListWidgetItem* item : items)
    {
        Highlighting h;
        h.pattern = item->text();
        h.color = item->data(Qt::UserRole).value<QColor>();
        list.append(h);
    }
    return list;
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
