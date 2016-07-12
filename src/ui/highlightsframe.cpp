#include <QDebug>
#include <QColorDialog>
#include <QKeyEvent>
#include <QDataStream>

#include "ui_highlightsframe.h"
#include "ui/highlightsframe.h"

CHighlightsFrame::CHighlightsFrame(QWidget* parent)
    : QFrame(parent),
    m_ui(new Ui::CHighlightsFrame),
    m_color(QColor(Qt::red))
{
    m_ui->setupUi(this);
    hide();
    refreshColorButton();

    qRegisterMetaTypeStreamOperators<Highlighting>("Highlighting");
}

CHighlightsFrame::~CHighlightsFrame()
{
    delete m_ui;
}

void CHighlightsFrame::addHighlighting()
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

void CHighlightsFrame::addHighlighting(QListWidgetItem* item)
{
    m_ui->filterList->addItem(item);
}

void CHighlightsFrame::showEvent(QShowEvent* event)
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

void CHighlightsFrame::onTextEdited(const QString& text)
{
    m_ui->btnAdd->setEnabled(!text.isEmpty());
}

void CHighlightsFrame::onSelectionChanged()
{
    QList<QListWidgetItem*> items = m_ui->filterList->selectedItems();
    qDebug() << "COUNT " << items.count();
    m_ui->btnDelete->setEnabled(!items.isEmpty());
}

void CHighlightsFrame::changeColor()
{
    QColor color = QColorDialog::getColor(QColor("red"), this);
    if (color.isValid())
    {
        m_color = color;
        refreshColorButton();
    }
}

void CHighlightsFrame::refreshColorButton()
{
    m_ui->btnColor->setStyleSheet(QString("background-color: %1;").arg(m_color.name()));
}

void CHighlightsFrame::deleteHighlighting()
{
    qDebug() << "DELETE";
    QListWidgetItem* item = m_ui->filterList->currentItem();
    delete item;

    if (m_ui->filterList->count() == 0)
    {
        m_ui->btnDeleteAll->setEnabled(false);
    }

    emit highlightingChanged();
}

void CHighlightsFrame::deleteAll()
{
    m_ui->filterList->clear();
    m_ui->btnDeleteAll->setEnabled(false);
}

void CHighlightsFrame::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        hide();
    }

    QFrame::keyPressEvent(event);
}

void CHighlightsFrame::clear()
{
    m_ui->filterList->clear();
}

QList<CHighlightsFrame::Highlighting> CHighlightsFrame::getItems()
{
    QList<QListWidgetItem*> items = m_ui->filterList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QList<Highlighting>     list;

    for (QListWidgetItem* item: items)
    {
        Highlighting h;
        h.pattern = item->text();
        h.color = item->data(Qt::UserRole).value<QColor>();
        list.append(h);
    }
    return list;
}

QDataStream& operator<<(QDataStream& out, const CHighlightsFrame::Highlighting& v)
{
    out << v.pattern << v.color;
    return out;
}

QDataStream& operator>>(QDataStream& in, CHighlightsFrame::Highlighting& v)
{
    in >> v.pattern;
    in >> v.color;
    return in;
}

// EOF <stefan@scheler.com>
