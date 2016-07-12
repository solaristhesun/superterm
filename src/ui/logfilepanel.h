#ifndef LOGFILEPANEL_H
#define LOGFILEPANEL_H

#include <QFrame>

namespace Ui {
class LogfilePanel;
}

class LogfilePanel : public QFrame
{
    Q_OBJECT

public:
    explicit LogfilePanel(QWidget* parent = 0);
    ~LogfilePanel();

    QString getLogFileName() const;

public slots:
    void showFileDialog();
    void onFileNameChanged(const QString& text);
    void onStartButtonClicked();

signals:
    void loggingStarted();
    void loggingStopped();

private:
    Ui::LogfilePanel* m_ui;
    bool              m_bEnabled;
};

#endif // LOGFILEPANEL_H
