#include "ui/widgets/clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    // currently empty
}

ClickableLabel::~ClickableLabel()
{
    // currently empty
}

void ClickableLabel::mousePressEvent(QMouseEvent*)
{
    emit clicked();
}

// EOF <stefan@scheler.com>
