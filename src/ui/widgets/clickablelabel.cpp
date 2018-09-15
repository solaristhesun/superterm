#include "clickablelabel.h"

CClickableLabel::CClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    // currently empty
}

CClickableLabel::~CClickableLabel() {}

void CClickableLabel::mousePressEvent(QMouseEvent*)
{
    emit clicked();
}
