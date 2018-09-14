#include <QMoveEvent>
#include <QDebug>
#include <QRect>

#if defined(Q_OS_WIN)
#include "windows.h"
#include "math.h"
#endif

#include "ui_mainwindow.h"
#include "ui/mainwindow.h"
#include "ui/consoletab.h"
#include "ui/consoletabbar.h"
#include "ui/consoletabfactory.h"
#include "misc/globals.h"
#include "misc/softwareversion.h"
#include "session/session.h"

CMainWindow::CMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::CMainWindow)
{
    qDebug() << "CMainWindow::CMainWindow()";

    m_ui->setupUi(this);
    QWidget::resize(800, 600);

    setWindowTitle(Globals::ApplicationFullName);
}

CMainWindow::~CMainWindow()
{
    qDebug() << "CMainWindow::~CMainWindow()" << this;
    delete m_ui;
}

QSize CMainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void CMainWindow::addExistingTabsFromFile()
{
    QStringList fileNames = CSession::getSessionList();

    if (!fileNames.isEmpty())
    {
        for (const QString& fileName : fileNames)
        {
            CSession* session = CSession::createSessionFromFile(fileName);

            m_ui->tabWidget->addTab(CConsoleTabFactory::createTabFromSession(session));
        }
    }
    else
    {
        m_ui->tabWidget->addTab(CConsoleTabFactory::createTab());
    }
}

void CMainWindow::attachTab(CConsoleTab* tab)
{
    qDebug() << "attaching tab" << tab->getLabel() << "to" << this;

    m_ui->tabWidget->addTab(tab);
}

CConsoleTab* CMainWindow::detachTab()
{
    int curIndex = m_ui->tabWidget->currentIndex();

    CConsoleTab* tab = m_ui->tabWidget->widget(curIndex);
    m_ui->tabWidget->removeTab(curIndex);

    qDebug() << "detaching tab" << tab->getLabel() << "from" << this;

    return tab;
}

void CMainWindow::showUpdateInfo(const SoftwareVersion& version)
{
    qDebug() << "version available " << version.toString();

    QMainWindow::setWindowTitle(QMainWindow::windowTitle() + " [update available]");
}

void CMainWindow::onSecondaryInstanceLaunched()
{
    qDebug() << "[slot] onSecondaryInstanceLaunched";

    CMainWindow* mainWindow = new CMainWindow;

    mainWindow->show();
    mainWindow->attachTab(CConsoleTabFactory::createTab());
}

bool CMainWindow::nativeEvent(const QByteArray& eventType, void* message, long*)
{
    bool bEventHandled = false;

#if defined(Q_OS_WIN)
    if (eventType == "windows_generic_MSG")
    {
        MSG* msg = static_cast< MSG* >( message );

        switch ( msg->message )
        {
            case WM_SIZING:
                {
                    RECT* r = reinterpret_cast<RECT*>(msg->lParam);

                    long widthWindow = r->right - r->left;
                    long heightWindow = r->bottom - r->top;

                    qDebug() << widthWindow << heightWindow;
                    CConsoleTab* tab = m_ui->tabWidget->widget(0); // FIXME: ugly

                    QSize decorationSize = QWidget::frameSize() - QWidget::size();
                    QSize charSize = tab->getCharSize();

                    int tabBarWidth = QWidget::width() - tab->getViewPortSize().width();

                    // calculate adjusted view dimensions
                    int adjustedViewWidth = floor((widthWindow - decorationSize.width() - tabBarWidth) / charSize.width()) * charSize.width() + 3;
                    int adjustedViewHeight = floor((heightWindow - decorationSize.height()) / charSize.height()) * charSize.height() + 3;

                    // manipulate event
                    r->right = r->left + decorationSize.width() + tabBarWidth + adjustedViewWidth;
                    r->bottom = r->top + decorationSize.height() + adjustedViewHeight;
                }
                break;
            default:
                break;
        }
    }
#elif defined(Q_OS_LINUX)
    if (eventType == "xcb_generic_event_t")
    {
        Q_UNUSED(message);
    }
#else
    Q_UNUSED(eventType)
    Q_UNUSED(message);
#endif

    return bEventHandled;
}

int CMainWindow::getTabCount() const
{
    return m_ui->tabWidget->count();
}

QRect CMainWindow::getTabBarRect() const
{
    CConsoleTabBar* tabBar = static_cast<CConsoleTabBar*>(m_ui->tabWidget->tabBar());
    return QRect(tabBar->mapToGlobal(QPoint(0, 0)), tabBar->size());
}

// EOF <stefan@scheler.com>
