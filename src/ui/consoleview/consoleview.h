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

#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QKeyEvent>
#include <QListView>
#include <QObject>
#include <QPen>

namespace Ui
{
class ConsoleView;
}

class ConsoleLineItemDelegate;

class ConsoleView : public QListView
{
    Q_OBJECT

public:
    explicit ConsoleView(QWidget* parent = nullptr);
    ~ConsoleView() override;

    void paintEvent(QPaintEvent* event) override;

    void setModel(QAbstractItemModel* model) override;

    void resizeEvent(QResizeEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void setFont(const QFont& font);

    QSize getCharWidth() const;

    void setTimestampsEnabled(const bool bTimestampsEnabled);
    bool timestampsEnabled() const;

    void   setTextColor(const QColor& color);
    QColor textColor() const;

    void   setBackgroundColor(const QColor& color);
    QColor backgroundColor() const;

    void setAutoScrollToBottom(const bool bAutoScrollToBottom);
    bool autoScrollToBottom() const;

public slots:
    void onRowsInserted(QModelIndex, int, int);
    void onDataChanged(QModelIndex, QModelIndex, QVector<int>);
    void scrollToBottom();

private:
    void paintTimestampsArea(QPainter& painter, const QRect& rect);
    void copySelectionToClipboard();

private:
    Ui::ConsoleView*         ui_;
    ConsoleLineItemDelegate* itemDelegate_;
    QBrush                   backgroundBrush_;
    QBrush                   timestampBrush_;
    QPen                     timestampPen_;
    bool                     bTimestampsEnabled_;
    bool                     bAutoScrollToBottom_;
    QColor                   textColor_;
    bool                     bAutoScrollTriggered;
};

#endif // CONSOLEVIEW_H
