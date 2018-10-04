#ifndef RENAMETABFRAME_H
#define RENAMETABFRAME_H

#include <QFrame>

namespace Ui
{
class RenameTabFrame;
}

class RenameTabFrame : public QFrame
{
    Q_OBJECT

public:
    explicit RenameTabFrame(QWidget* parent = nullptr);
    ~RenameTabFrame();

    void    setTabLabel(const QString& text);
    QString tabLabel() const;
    void    show();

signals:
    void applyPressed();

private slots:
    void onApplyClicked();

private:
    Ui::RenameTabFrame* ui_;
};

#endif // RENAMETABFRAME_H
