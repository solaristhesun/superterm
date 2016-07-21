#include <QLabel>

#include "gmock/gmock.h"
#include "ui/statusbarframe.h"

TEST(CStatusBarFrameTests, showMessage)
{
    CStatusBarFrame frame;

    frame.showMessage("test message");

    QLabel* label = frame.findChild<QLabel*>("labelMessage");

    ASSERT_EQ(QString("test message"), label->text());
}

// EOF <stefan@scheler.com>
