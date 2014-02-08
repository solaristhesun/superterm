#include <QDebug>

#include "highlightsframe.h"
#include "ui_highlightsframe.h"

CHighlightsFrame::CHighlightsFrame(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::CHighlightsFrame)
{
    m_ui->setupUi(this);

    hide();
}

CHighlightsFrame::~CHighlightsFrame()
{
    delete m_ui;
}

void CHighlightsFrame::addHighlighting(void)
{
    const QString str = m_ui->filterEdit->text();

    if (!str.isEmpty())
    {
        m_ui->filterList->addItem(str);
        m_ui->filterEdit->setText("");
        m_ui->filterEdit->setFocus();
    }
}

void CHighlightsFrame::showEvent(QShowEvent * event)
{
    m_ui->filterEdit->setText("");
    m_ui->filterEdit->setFocus();

    QFrame::showEvent(event);
}

void CHighlightsFrame::onTextEdited(const QString& text)
{
    m_ui->btnAdd->setEnabled(!text.isEmpty());
}

void CHighlightsFrame::onSelectionChanged()
{
    QList<QListWidgetItem *> items = m_ui->filterList->selectedItems();
    qDebug() << "COUNT " << items.count();
    m_ui->btnDelete->setEnabled(!items.isEmpty());
}

void CHighlightsFrame::deleteHighlighting(void)
{
    qDebug() << "DELETE";
    QListWidgetItem *item = m_ui->filterList->currentItem();
    delete item;
}

void CHighlightsFrame::hideFrame(void)
{
    hide();
    emit closed();
}

QStringList CHighlightsFrame::getItems(void)
{
    QList<QListWidgetItem *> items = m_ui->filterList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QStringList list;

    foreach(QListWidgetItem *item, items)
        list.append(item->text());

    return list;
}

// EOF <stefan@scheler.com>
