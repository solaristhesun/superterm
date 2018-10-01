#ifndef LOGFILEPANEL_H
#define LOGFILEPANEL_H

#include <QFrame>

namespace Ui
{
class LogfilePanel;
}

class LogfilePanel : public QFrame
{
    Q_OBJECT

public:
    explicit LogfilePanel(QWidget* parent = nullptr);
    ~LogfilePanel();

    QString getLogFileName() const;
    void setLogFileName(QString fileName);

public slots:
    void showFileDialog();
    void onFileNameChanged(const QString& text);
    void onStartButtonClicked();

signals:
    void loggingStarted();
    void loggingStopped();

private:
    Ui::LogfilePanel* ui_;
    bool              bEnabled_;
};

#endif // LOGFILEPANEL_H
